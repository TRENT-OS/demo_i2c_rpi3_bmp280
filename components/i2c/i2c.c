/*
 *  I2C Interface  
 *
 *  Copyright (C) 2022, HENSOLDT Cyber GmbH
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "OS_Dataport.h"

#include "lib_debug/Debug.h"
#include "plat_i2c.h"

#include "i2c.h"

#include <camkes.h>
#include <camkes/io.h>

static struct 
{
    bool    init_ok;
    OS_Dataport_t   port_storage;
} ctx =
{
    .init_ok = false,
    .port_storage = OS_DATAPORT_ASSIGN(i2c_port)
};

void post_init(void);

/*void i2c_init(void)
{
    post_init();
}
*/
void post_init(void)
{
    Debug_LOG_DEBUG("[%s] %s", get_instance_name(), __func__);
    if(! _i2c_init())
    {
        Debug_LOG_ERROR("_i2c_init() failed, Platform initialisation could not be performed for %s", get_instance_name());
        return;
    }

    ctx.init_ok = true;
    Debug_LOG_INFO("%s done", get_instance_name());
}

I2C_Error_t i2c_rpc_init_slave(int dev)
{
    return I2C_ERROR_NOT_IMPLEMENTED;
}


I2C_Error_t i2c_rpc_write(int dev, size_t len, size_t *written)
{
    *written = 0;
    if(!ctx.init_ok)
    {
        Debug_LOG_ERROR("I2C is not yet initialised");
        return I2C_ERROR_NOT_INITIALISED;
    }
    size_t dataport_size = OS_Dataport_getSize(ctx.port_storage);
    if(len > dataport_size)
    {
        // the client did a bogus request, it knows the data port size and
        // never ask for more data
        Debug_LOG_ERROR(
            "i2c_rpc_write() size %zu exceeds dataport size %zu",
            len,
            dataport_size
        );
        return I2C_ERROR_INVALID_PARAMETER;
    }
    const uint8_t* buf = (uint8_t*) OS_Dataport_getBuf(ctx.port_storage);


    return _i2c_write(dev, buf, len, written);
}


I2C_Error_t i2c_rpc_read(int dev, size_t len, size_t *read)
{
    *read = 0;
    if(!ctx.init_ok)
    {
        Debug_LOG_ERROR("I2C is not yet initialised");
        return I2C_ERROR_NOT_INITIALISED;
    }
    size_t dataport_size = OS_Dataport_getSize(ctx.port_storage);
    if(len > dataport_size)
    {
        // the client did a bogus request, it knows the data port size and
        // never ask for more data
        Debug_LOG_ERROR(
            "i2c_rpc_read() size %zu exceeds dataport size %zu",
            len,
            dataport_size
        );
        return I2C_ERROR_INVALID_PARAMETER;
    }
     uint8_t* buf = (uint8_t*) OS_Dataport_getBuf(ctx.port_storage);

    return _i2c_read(dev, buf, len, read);
}
