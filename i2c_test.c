/*
 * Copyright (C) 2022 HENSOLDT Cyber GmbH
 */


#include "OS_Error.h"
#include "lib_debug/Debug.h"
#include "i2c.h"
#include "bmp280.h"
#include <stdint.h>
#include <stdbool.h>
//#include "OS_Dataport.h"

#include <camkes.h>



#define DEVICE (0x76 << 1)


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

void wait(void)
{
    for (int i = 0; i < 100000 * 2000; i++)
    {
        __asm__("nop");
    }
}

OS_Error_t run(void)
{
    OS_Error_t err = OS_SUCCESS;
    I2C_Error_t i2c_err = I2C_SUCCESS;
    size_t tmp = 0;

    i2c_init();
    
    uint8_t buf[20];
    buf[0] = 0xd0;

    i2c_err = i2c_write(DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_write() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }

    i2c_err = i2c_write(DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_read() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }   

    Debug_LOG_INFO("ID of bmp280 is 0x%x", buf[0]);
    buf[0] = 0xf4;
     i2c_err = i2c_write(DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_write() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }

    i2c_err = i2c_write(DEVICE, 1, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_read() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }  

    Debug_LOG_INFO("ctrl_meas dump is 0x%x", buf[0]);

    bmp280_params_t params;


    bmp280_init_default_params(&params);

    bmp280_t bmp280_dev;
    bmp280_dev.i2c_dev.bus = 0;
    bmp280_dev.i2c_dev.addr = BMP280_I2C_ADDRESS_0;

    bmp280_init(&bmp280_dev, &params);
    
    for(int i = 0; i < 20; i++)
    {
        wait();
        getSensorData(&bmp280_dev);
    }
    
    /*    buf[0] = 0xf4;
    buf[1] = 0xa7;
    i2c_err = i2c_write(0x76, 2, &tmp, buf);
    if(i2c_err != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_read() returned errorcode %d", i2c_err);
        err = OS_ERROR_ABORTED;
        return err;
    }  
    
    if(tmp != 2)
    {
        Debug_LOG_WARNING("i2c_write() failed to write all data");
        return err;
    }
*/

    Debug_LOG_INFO("Done");
    return err;
}