/*
 *  I2C Interface  
 *
 *  Copyright (C) 2022, HENSOLDT Cyber GmbH
 *  SPDX-License-Identifier: BSD-3-Clause
 */


#include "lib_debug/Debug.h"
#include "rpi3_i2c.h"

#include "i2c.h"

#include <camkes.h>

static struct 
{
    bool    init_ok;
    //OS_Dataport_t   port_storage;
} ctx =
{
    //.port_storage = OS_DATAPORT_ASSIGN(storage_port)
    .init_ok = false
};

void post_init(void);

void i2c_init(void)
{
    post_init();
}

void post_init(void)
{

    if(! _i2c_init())
    {
        Debug_LOG_ERROR("_i2c_init() failed, Platform initialisation could not be performed");
        return;
    }

    ctx.init_ok = true;
    Debug_LOG_INFO("I2C initialised");
}


I2C_Error_t i2c_write(int dev, size_t len, size_t *written, const uint8_t* buf)
{
    if(!ctx.init_ok)
    {
        Debug_LOG_ERROR("I2C is not yet initialised");
        return I2C_ERROR_NOT_INITIALISED;
    }
    // Do some generic checks here
    return _i2c_write(dev, buf, len, written);
}


I2C_Error_t i2c_read(int dev, size_t len, size_t *read, const uint8_t* buf)
{
    if(!ctx.init_ok)
    {
        Debug_LOG_ERROR("I2C is not yet initialised");
        return I2C_ERROR_NOT_INITIALISED;
    }
    // Do some generic checks here
    return _i2c_read(dev, buf, len, read);
}
