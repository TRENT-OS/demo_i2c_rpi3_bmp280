
function(SSD1306_Declare_CAmkESComponent name)

DeclareCAmkESComponent(
    ${name}
    INCLUDES
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/ssd1306
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/fonts
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../i2c/include
    SOURCES
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/ssd1306/ssd1306.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/fonts/fonts.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/ssd1306_rpc.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../i2c_lib.c
    LIBS
    system_config
    lib_debug
    os_core_api
)

endfunction()