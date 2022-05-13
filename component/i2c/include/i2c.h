


#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum {
    I2C_ERROR_TRY_AGAIN = -6,
    I2C_ERROR_NOT_INITIALISED,
    I2C_ERROR_INVALID_ADDRESS,
    I2C_ERROR_CLK_TIMEOUT,
    I2C_ERROR_NACK,
    I2C_ERROR_GENERIC,

    //---------------------------
    I2C_SUCCESS
}
I2C_Error_t;

// tmp, to be removed
void i2c_init(void);


I2C_Error_t i2c_write(int dev, size_t len, size_t *written, const uint8_t* buf);


I2C_Error_t i2c_read(int dev, size_t len, size_t *read, const uint8_t* buf);