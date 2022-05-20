
#pragma once

#include "if_i2c.h"

#include "arch/arm/i2c.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_I2C_SLAVES 20

bool _i2c_init(void);

I2C_Error_t _i2c_init_slave(int dev);

I2C_Error_t _i2c_write(int dev, const uint8_t* buf, size_t len, size_t* written);


I2C_Error_t _i2c_read(int dev, uint8_t *buf, size_t len, size_t *read);