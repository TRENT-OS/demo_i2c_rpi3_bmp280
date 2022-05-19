

#include "bmp280.h"
#include <camkes.h>
#include <stdbool.h>

#include "i2c_lib.h"

#include "lib_debug/Debug.h"

static bmp280_t bmp280_dev = {
    .i2c_dev.bus = IF_I2C_ASSIGN(i2c_rpc, i2c_port, i2cBus_notify),
    .i2c_dev.addr = BMP280_I2C_ADDRESS_0,
};

void post_init(void)
{
    Debug_LOG_DEBUG("[%s] %s running", get_instance_name(), __func__);
    bmp280_params_t params;
    bmp280_init_default_params(&params);

    bmp280_init(&bmp280_dev, &params);
    Debug_LOG_INFO("bmp280 initialised");
}

void bmp280_rpc_get_data(float* temperature, float* pressure, float* humidity)
{
    bmp280_read_float(&bmp280_dev, temperature, pressure, humidity);

    bool bme280p = bmp280_dev.id == BME280_CHIP_ID;
    if(!bme280p)
    {
        *humidity = 0.0;
    }
}