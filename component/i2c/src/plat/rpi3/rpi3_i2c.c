
#include "lib_debug/Debug.h"

#include "bcm2837_gpio.h"
#include "bcm2837_i2c.h"
#include "rpi3_i2c.h"



#include <camkes.h>


bool _i2c_init(void)
{
    if(!bcm2837_i2c_begin(regBase))
    {
        Debug_LOG_ERROR("bcm2837_i2c_begin() failed");
        return false;
    }
    return true;
}

I2C_Error_t _i2c_write(int dev, uint8_t* buf, size_t len, size_t* written)
{
    int ret = 0;
    *written = 0;

    bcm2837_i2c_setSlaveAddress((uint8_t) (dev >> 1));

    ret = bcm2837_i2c_write((char*) buf, len);

    if(ret == BCM2837_I2C_REASON_ERROR_NACK)
    {
        Debug_LOG_INFO("bcm2837_i2c_write() returned NACK error");
        return I2C_ERROR_NACK;
    }
    if(ret == BCM2837_I2C_REASON_ERROR_CLKT)
    {
        Debug_LOG_INFO("bcm2837_i2c_write() returned clock stretching timeout");
        return I2C_ERROR_CLK_TIMEOUT;        
    }
    if(ret < 0)
    {
        Debug_LOG_WARNING("bcm2837_i2c_write() returned Unsupported error %d", ret);
        return I2C_ERROR_GENERIC;      
    }
    *written = len - ret;
    if(ret != len)
    {
        Debug_LOG_INFO("bcm2837_i2c_write() write transaction not completed");
        return I2C_ERROR_TRY_AGAIN;
    }
    return I2C_SUCCESS;
}


I2C_Error_t _i2c_read(int dev, uint8_t *buf, size_t len, size_t *read)
{
    int ret = 0;
    *read = 0;

    bcm2837_i2c_setSlaveAddress((uint8_t) (dev >> 1));

    ret = bcm2837_i2c_read((char*) buf, len);

    if(ret == BCM2837_I2C_REASON_ERROR_NACK)
    {
        Debug_LOG_INFO("bcm2837_i2c_read() returned NACK error");
        return I2C_ERROR_NACK;
    }
    if(ret == BCM2837_I2C_REASON_ERROR_CLKT)
    {
        Debug_LOG_INFO("bcm2837_i2c_read() returned clock stretching timeout");
        return I2C_ERROR_CLK_TIMEOUT;        
    }
    if(ret < 0)
    {
        Debug_LOG_WARNING("bcm2837_i2c_read() returned Unsupported error %d", ret);
        return I2C_ERROR_GENERIC;      
    }
    *read = len - ret;
    if(ret != len)
    {
        Debug_LOG_INFO("bcm2837_i2c_read() read transaction not completed");
        return I2C_ERROR_TRY_AGAIN;
    }
    return I2C_SUCCESS;
}


//----------------------------------------------------
// Debug Functions
//----------------------------------------------------

void print_i2c_regs(void)
{
    volatile uint32_t *bcm2837_bsc = (uint32_t*)(regBase + (I2C_BASE_ADDRESS - BCM2837_GPIO_BASE));
    
    Debug_LOG_INFO("Base address is %p", bcm2837_bsc);
    Debug_LOG_INFO("Controll register at %p : 0x%x, add", bcm2837_bsc, *bcm2837_bsc);
    bcm2837_bsc++;
    Debug_LOG_INFO("Status register at %p : 0x%x, add", bcm2837_bsc, *bcm2837_bsc);
    bcm2837_bsc++;
    Debug_LOG_INFO("dlen register at %p : 0x%x, add", bcm2837_bsc, *bcm2837_bsc);
    bcm2837_bsc++;
    Debug_LOG_INFO("Address register at %p : 0x%x, add", bcm2837_bsc, *bcm2837_bsc);
    bcm2837_bsc++;
    Debug_LOG_INFO("FIFO register at %p : 0x%x, add", bcm2837_bsc, *bcm2837_bsc);
    bcm2837_bsc++;
    Debug_LOG_INFO("div register at %p : 0x%x, add", bcm2837_bsc, *bcm2837_bsc);
    bcm2837_bsc++;
    Debug_LOG_INFO("DEL register at %p : 0x%x, add", bcm2837_bsc, *bcm2837_bsc);
    bcm2837_bsc++;
    Debug_LOG_INFO("CLKT register at %p : 0x%x, add", bcm2837_bsc, *bcm2837_bsc);
    bcm2837_bsc++;
}


void print_gpio_regs(void)
{
    volatile uint32_t *bcm2837_gpio = (uint32_t*)regBase;

    Debug_LOG_INFO("Base address is %p", bcm2837_gpio);

    Debug_LOG_INFO("GPFSEL0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPFSEL1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPFSEL2 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPFSEL3 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPFSEL4 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPFSEL5 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++; // Reserved
    Debug_LOG_INFO("GPSET0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPSET10 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++; // Reserved
    Debug_LOG_INFO("GPCLR0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPCLR1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPLEV0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPLEV1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPEDS0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPEDS1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPREN0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPREN1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPFEN0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPFEN1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPHEN0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPHEN1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPLEN0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPLEN1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPAREN0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPAREN1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPAFEN0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPAFEN1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    bcm2837_gpio++;
    Debug_LOG_INFO("GPPUD register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPPUDCLK0 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
    Debug_LOG_INFO("GPPPUDCLK1 register at %p: 0x%x", bcm2837_gpio, *bcm2837_gpio);
    bcm2837_gpio++;
}