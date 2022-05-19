
function(BMP280_DeclareCAmkESComponent name)

DeclareCAmkESComponent(
    ${name}
    INCLUDES
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/include
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../i2c/include
    SOURCES
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/src/bmp280.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/bmp280_rpc.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../i2c_lib.c
    LIBS
    system_config
    lib_debug
    os_core_api
)

endfunction()