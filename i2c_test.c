/*
 * Copyright (C) 2022 HENSOLDT Cyber GmbH
 */


#include "OS_Error.h"
#include "bcm2837_i2c.h"
#include "lib_debug/Debug.h"
#include <stdint.h>

#include <camkes.h>

OS_Error_t run(void)
{
    OS_Error_t err = OS_SUCCESS;
    uint8_t ret;

    char buf_red[20];
    char cmd[] = {0x6f};

    Debug_LOG_INFO("Initialise I2C");
    if (!bcm2837_i2c_begin(regBase))
    {
        Debug_LOG_ERROR("Initialisation of I2C failed");
    }

    bcm2837_i2c_setSlaveAddress(0b11101100);
    
    ret = bcm2837_i2c_write_read_rs(cmd, 1, buf_red, 2);

    Debug_LOG_INFO("value of ret was %i, buf contained %u %u", ret, buf_red[0], buf_red[1]);
    return err;
}