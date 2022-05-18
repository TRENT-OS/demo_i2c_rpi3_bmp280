/*
 * Copyright (C) 2022 HENSOLDT Cyber GmbH
 */


#include "OS_Error.h"
#include "lib_debug/Debug.h"
#include "i2c.h"
#include "ssd1306.h"
#include "bmp280.h"
#include <stdint.h>
#include <stdbool.h>
#include "fonts.h"
//#include "OS_Dataport.h"

#include <camkes.h>


#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

#define DEVICE (0x76 << 1)


static uint8_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

void getSensorData(bmp280_t* bmp280_dev)
{
    bool bme280p = bmp280_dev->id == BME280_CHIP_ID;
    float pressure, temperature, humidity;

    bmp280_read_float(bmp280_dev, &temperature, &pressure, &humidity);
    Debug_LOG_INFO("Pressure: %.2f Pa, Temperature: %.2f C", pressure, temperature);
    if (bme280p)
    {
        Debug_LOG_INFO("Humidity: %.2f %%", humidity);
    }
}

void writeDatatoDisplay(ssd1306_t* ssd1306_dev, char* str, char* einheit ,float value)
{
    char buf[20];
    const font_info_t *font = NULL;
    font_face_t font_face = FONT_FACE_GLCD5x7;
    int ret = 0;

    font = font_builtin_fonts[font_face];
    ret = ssd1306_fill_rectangle(ssd1306_dev, buffer, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, OLED_COLOR_BLACK);
    if(ret != 0)
    {
        Debug_LOG_ERROR("ssd1306_fill_rectangle() returned %d", ret);
    }
    ret = ssd1306_draw_string(ssd1306_dev, buffer, font, 20, 0, str, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    if(ret < 0)
    {
        Debug_LOG_ERROR("ssd1306_draw_string() with GLCD5X7 Font returned %d", ret);
    }
    sprintf(buf," %.1f %s", value, einheit);
    font = font_builtin_fonts[FONT_FACE_TERMINUS_10X18_ISO8859_1];
    ret = ssd1306_draw_string(ssd1306_dev, buffer, font, 15, 10, buf, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    if(ret < 0)
    {
        Debug_LOG_ERROR("ssd1306_draw_string() with TERMINUS_10x18 returned %d", ret);
    }
    ret = ssd1306_load_frame_buffer(ssd1306_dev, buffer);
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

void runDemo(bmp280_t* bmp280_dev, ssd1306_t* ssd1306_dev)
{
    float pressure, temperature, humidity;

    while(1)
    {
        bmp280_read_float(bmp280_dev, &temperature, &pressure, &humidity);
        writeDatatoDisplay(ssd1306_dev,"Temperatur: ", "C", temperature);
        wait();
        writeDatatoDisplay(ssd1306_dev,"Luftdruck: ", "hPa", pressure/100 );
        wait();
        writeDatatoDisplay(ssd1306_dev,"Luftfeuchte: ", "%", humidity);
        wait();
    }
}

OS_Error_t Test_SSD1306(ssd1306_t* dev)
{
    const font_info_t *font = NULL;
    font_face_t font_face = FONT_FACE_GLCD5x7;
    int drawn_char = 0;
    OS_Error_t err = OS_SUCCESS;

    font = font_builtin_fonts[font_face];
    if (ssd1306_fill_rectangle(dev, buffer, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT/2, OLED_COLOR_WHITE) != 0)
    {
        Debug_LOG_ERROR("SSD1306_fill_rectangle() failed!");
        err = OS_ERROR_ABORTED;
        return err;
    }
    if(ssd1306_load_frame_buffer(dev, buffer))
    {
        Debug_LOG_ERROR("SSD1306_load_frame_buffer() failed!");
        err = OS_ERROR_ABORTED;
        return err;
    }
    //wait();
    //ssd1306_display_on(&dev, true);
    ssd1306_set_whole_display_lighting(dev, false);
    wait();
    ssd1306_fill_rectangle(dev, buffer, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, OLED_COLOR_BLACK);
    ssd1306_load_frame_buffer(dev, buffer);
    wait();
    drawn_char = ssd1306_draw_string(dev, buffer, font, 0, 0, "Hello, SeL4!", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    Debug_LOG_INFO("ssd1306_draw_string() returned %i", drawn_char);
    if(ssd1306_load_frame_buffer(dev, buffer))
    {
        Debug_LOG_ERROR("SSD1306_load_frame_buffer() failed!");
        err = OS_ERROR_ABORTED;
        return err;
    }
    wait();
    font_face = FONT_FACE_TERMINUS_10X18_ISO8859_1;
    font = font_builtin_fonts[font_face];
    drawn_char = ssd1306_draw_string(dev, buffer, font, 0, 0, "Hello, SeL4!", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    Debug_LOG_INFO("ssd1306_draw_string() returned %i", drawn_char);
    if(ssd1306_load_frame_buffer(dev, buffer))
    {
        Debug_LOG_ERROR("SSD1306_load_frame_buffer() failed!");
        err = OS_ERROR_ABORTED;
        return err;
    }
    wait();
    return err;
}

OS_Error_t run(void)
{
    OS_Error_t err = OS_SUCCESS;
    I2C_Error_t i2c_err = I2C_SUCCESS;
    size_t tmp = 0;

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

    i2c_err = i2c_write(&bus, DEVICE, 1, &tmp, buf);
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

    i2c_err = i2c_write(&bus, DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_read() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }  

    Debug_LOG_INFO("ctrl_meas dump is 0x%x", buf[0]);

    bmp280_params_t params;


    bmp280_init_default_params(&params);

    bmp280_t bmp280_dev = {
    .i2c_dev.bus = IF_I2C_ASSIGN(i2c_rpc, i2c_port, i2cBus_notify),
    .i2c_dev.addr = BMP280_I2C_ADDRESS_0,
    };

    bmp280_init(&bmp280_dev, &params);
    Debug_LOG_INFO("BMP280 Initialised, now readout some data");
    for(int i = 0; i < 5; i++)
    {
        getSensorData(&bmp280_dev);
    }
    Debug_LOG_INFO("BMP280 Test completed");
    ssd1306_t ssd1306_dev = {
        .protocol = SSD1306_PROTO_I2C,
        .screen = SSD1306_SCREEN,
        .i2c_dev.bus = IF_I2C_ASSIGN(i2c_rpc, i2c_port, i2cBus_notify),
        .i2c_dev.addr = SSD1306_I2C_ADDR_0,
        .width = DISPLAY_WIDTH,
        .height = DISPLAY_HEIGHT
    };

    if(ssd1306_init(&ssd1306_dev) != 0)
    {
        Debug_LOG_ERROR("SSD1306 Initialisation failde!");
        err = OS_ERROR_ABORTED;
        return err;
    }
    Debug_LOG_INFO("SSD1306 Initialised, write hello world on screen");
    if (Test_SSD1306(&ssd1306_dev) != OS_SUCCESS)
    {
        err = OS_ERROR_ABORTED;
        return err;
    }
    Debug_LOG_INFO("SSD1306 test completed, now running Demo!");
    runDemo(&bmp280_dev, &ssd1306_dev);
    
    Debug_LOG_INFO("Done");
    return err;
}