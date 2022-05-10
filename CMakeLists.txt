#
# Demo Hello Word
#
# Copyright (C) 2019-2020, HENSOLDT Cyber GmbH
#

cmake_minimum_required(VERSION 3.7.2)


os_set_config_file(
    "system_config.h"
    CONFIG_PROJECT_NAME "system_config"
)

#-------------------------------------------------------------------------------
project(i2c_test C)

DeclareCAmkESComponent(
    I2CTest
    SOURCES
        i2c_test.c
        3rdParty/bcm2837/bcm2837_gpio.c
        3rdParty/bcm2837/bcm2837_i2c.c
    INCLUDES
        3rdParty/bcm2837
    C_FLAGS
        -Wall
        -Werror
    LIBS
        system_config
        lib_debug
        os_core_api
)


DeclareAndCreateCamkESSystem(i2c_test.camkes)
GenerateSimulateScript()