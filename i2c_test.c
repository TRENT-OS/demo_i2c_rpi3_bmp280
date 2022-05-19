/*
 * Copyright (C) 2022 HENSOLDT Cyber GmbH
 */


#include "OS_Error.h"
#include "lib_debug/Debug.h"
#include "i2c.h"
#include "ssd1306.h"
#include <stdint.h>
#include "fonts.h"
#include <string.h>

#include <camkes.h>


#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

#define DEVICE (0x76 << 1)


OS_Dataport_t ssd1306_port_storage = OS_DATAPORT_ASSIGN(ssd1306_port);

int ssd1306_write_string(char * str, int len,font_face_t font_face , uint8_t x, uint8_t y)
{
    uint8_t* ssd1306_buf = OS_Dataport_getBuf(ssd1306_port_storage);
    size_t ssd1306_buf_size = OS_Dataport_getSize(ssd1306_port_storage);
    if(*(str  + len) != '\0')
    {
        Debug_LOG_ERROR("C Stings need to terminate with a NULL character");
        return -1;
    }
    if(len + 1 > ssd1306_buf_size)
    {
        Debug_LOG_ERROR("String to big for shared memory");
        return -1;
    }
    memcpy(ssd1306_buf, str, len);
    return ssd1306_rpc_write_string(len, (int) font_face, x, y );

}

void writeDatatoDisplay( char* str, char* einheit ,float value)
{
    int ret;
    char buf[20];

    ssd1306_rpc_clear_display();
    ret = ssd1306_write_string(str,strlen(str),FONT_FACE_GLCD5x7, 20, 0);
    if(ret < 0)
    {
        Debug_LOG_ERROR("ssd1306_draw_string() with GLCD5X7 Font returned %d", ret);
    }
    sprintf(buf," %.1f %s", value, einheit);
    ret = ssd1306_write_string(buf, strlen(buf), FONT_FACE_TERMINUS_10X18_ISO8859_1, 15, 10);
    if(ret < 0)
    {
        Debug_LOG_ERROR("ssd1306_draw_string() with TERMINUS_10x18 returned %d", ret);
    }
    ret = ssd1306_rpc_update_display();
    if(ret != 0)
    {
        Debug_LOG_ERROR("ssd1306_load_frame_buffer() returned %d", ret);
    }
}

void wait(void)
{
    for (int i = 0; i < 100000 * 2000; i++)
    {
        __asm__("nop");
    }
}

void runDemo( void)
{
    float pressure, temperature, humidity;

    while(1)
    {
        bmp280_rpc_get_data( &temperature, &pressure, &humidity);
        writeDatatoDisplay("Temperatur: ", "C", temperature);
        wait();
        writeDatatoDisplay("Luftdruck: ", "hPa", pressure/100 );
        wait();
        writeDatatoDisplay("Luftfeuchte: ", "%", humidity);
        wait();
    }
}


OS_Error_t run(void)
{
    OS_Error_t err = OS_SUCCESS;
    I2C_Error_t i2c_err = I2C_SUCCESS;
    size_t tmp = 0;

    while(!bmp280_rpc_sensor_ready())
    {
        seL4_Yield();
    }

    if_I2C_t bus = IF_I2C_ASSIGN(i2c_rpc, i2c_port, i2cBus_notify);    
    uint8_t buf[20];
    buf[0] = 0xd0;

    i2c_err = i2c_write(&bus, DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_write() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }

    i2c_err = i2c_read(&bus, DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_read() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }   

    Debug_LOG_INFO("ID of bmp280 is 0x%x", buf[0]);
    buf[0] = 0xf4;
     i2c_err = i2c_write(&bus, DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_write() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }

    i2c_err = i2c_read(&bus, DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_read() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }  

    Debug_LOG_INFO("ctrl_meas dump is 0x%x", buf[0]);

    runDemo();
    
    Debug_LOG_INFO("Done");
    return err;
}