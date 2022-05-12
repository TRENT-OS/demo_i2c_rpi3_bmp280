
#pragma once

#include "i2c.h"

#include <stdint.h>
#include <stdbool.h>



bool _i2c_init(void);


I2C_Error_t _i2c_write(int dev, uint8_t* buf, size_t len, size_t* written);


I2C_Error_t _i2c_read(int dev, uint8_t *buf, size_t len, size_t *read);