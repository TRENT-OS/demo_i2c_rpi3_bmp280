


#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum {
    I2C_ERROR_TRY_AGAIN = -8,
    I2C_ERROR_NOT_IMPLEMENTED,
    I2C_ERROR_INVALID_PARAMETER,
    I2C_ERROR_NOT_INITIALISED,
    I2C_ERROR_INVALID_ADDRESS,
    I2C_ERROR_CLK_TIMEOUT,
    I2C_ERROR_NACK,
    I2C_ERROR_GENERIC,

    //---------------------------
    I2C_SUCCESS
}
I2C_Error_t;
