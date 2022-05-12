/*
 * Copyright (C) 2022 HENSOLDT Cyber GmbH
 */


#include "OS_Error.h"
#include "lib_debug/Debug.h"
#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>
//#include "OS_Dataport.h"

#include <camkes.h>



#define DEVICE (0x76 << 1)



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
    buf[0] = 0xf4;
    buf[1] = 0xa7;
    err = i2c_write(0x76, 2, &tmp, buf);
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
    Debug_LOG_INFO("Done");
    return err;
}