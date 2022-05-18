

#include "i2c.h"

#include <stdint.h>

I2C_Error_t i2c_write(const if_I2C_t* bus, int dev, size_t len, size_t *written, const uint8_t* buf);

I2C_Error_t i2c_write_reg(const if_I2C_t* bus, int dev, int reg, size_t len, size_t *written, const uint8_t* buf);

I2C_Error_t i2c_read(const if_I2C_t* bus, int dev, size_t len, size_t *read, uint8_t* buf);