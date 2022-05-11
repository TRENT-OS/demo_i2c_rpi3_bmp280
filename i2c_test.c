/*
 * Copyright (C) 2022 HENSOLDT Cyber GmbH
 */


#include "OS_Error.h"
#include "bcm2837_i2c.h"
#include "lib_debug/Debug.h"
#include <stdint.h>

#include <camkes.h>

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

OS_Error_t run(void)
{
    OS_Error_t err = OS_SUCCESS;
    uint8_t ret;

    char buf_red[20];
    char cmd[] = {0xd0};

    for(int i = 0; i < 20; i++)
    {
        buf_red[i] = 0;        
    }

    //print_gpio_regs();
    Debug_LOG_INFO("Initialise I2C");
    if (!bcm2837_i2c_begin(regBase))
    {
        Debug_LOG_ERROR("Initialisation of I2C failed");
    }
    //print_gpio_regs();

    bcm2837_i2c_setSlaveAddress(0x76);

    ret = bcm2837_i2c_write(cmd, 1);
    Debug_LOG_INFO("Wrote cmd, ret was %i", ret);
    ret = bcm2837_i2c_read( buf_red, 1);

    Debug_LOG_INFO("value of ret was %i, buf contained %u %u", ret, buf_red[0], buf_red[1]);

    //print_i2c_regs();

    return err;
}