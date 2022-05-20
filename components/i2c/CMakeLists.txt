

cmake_minimum_required(VERSION 3.17)

#-------------------------------------------------------------------------------
#
# Declare I2C driver CAmkES Component
#
# Parameters:
#
#   <name>
#       component name
#   <id>
#       ID used in platsupport libs

#


function(I2C_DeclareCAmkESComponent name id)
    set(dir_plat "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/plat/${PLATFORM}")
    set(test_dir "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    if(NOT IS_DIRECTORY "${dir_plat}")
        message(FATAL_ERROR "unsupported platform: '${PLATFORM}', '${dir_plat}'")  
    endif()
    if(EXISTS "${dir_plat}/i2c_lib.cmake")
        include("${dir_plat}/i2c_lib.cmake")
    endif()

    if(NOT I2C_LIB_SOURCE)
        set(I2C_LIB_SOURCE)
    endif()

    if(NOT I2C_LIB_INCLUDE)
        set(I2C_LIB_INCLUDE)
    endif()

    DeclareCAmkESComponent(
        ${name}
        INCLUDES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/plat/${PLATFORM}
            ${I2C_LIB_INCLUDE}
        SOURCES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/i2c.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/plat/${PLATFORM}/plat_i2c.c
            ${I2C_LIB_SOURCE}
        C_FLAGS
            -Wall
            -Werror
            -DI2C_CONFIG_ID=${id}
        LIBS
            os_core_api
            lib_debug
            platsupport
    )

endfunction()



function(DeclareCAmkESComponents_for_I2Cs)

    set(dir_plat "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/plat/${PLATFORM}")

    if(NOT IS_DIRECTORY "${dir_plat}")
        message(FATAL_ERROR "unsupported platform: '${PLATFORM}'")
    endif()

    CAmkESAddCPPInclude("${dir_plat}")
    include("${dir_plat}/plat_i2c.cmake")
    
endfunction()