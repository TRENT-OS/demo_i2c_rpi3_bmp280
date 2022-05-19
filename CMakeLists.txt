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

#        component/ssd1306/src/ssd1306.c
#        component/ssd1306/src/ssd1306_fonts.c

#        component/ssd1306/src

#-------------------------------------------------------------------------------


project(i2c_test C)
include(${CMAKE_CURRENT_SOURCE_DIR}/components/bmp280/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/components/ssd1306/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/components/i2c/CMakeLists.txt)
DeclareCAmkESComponents_for_I2Cs()
BMP280_DeclareCAmkESComponent(BMP280)
SSD1306_Declare_CAmkESComponent(SSD1306)
DeclareCAmkESComponent(
    I2CTest
    SOURCES
        i2c_test.c
        components/ssd1306/3rdParty/ssd1306/ssd1306.c
        components/ssd1306/3rdParty/fonts/fonts.c
        i2c_lib.c
    INCLUDES
        components/bmp280/include
        components/bmp280/3rdPary/include
        components/ssd1306/3rdParty/ssd1306
        components/ssd1306/3rdParty/fonts
        components/i2c/include
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