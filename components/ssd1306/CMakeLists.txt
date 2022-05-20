
function(SSD1306_Declare_CAmkESComponent name)

DeclareCAmkESComponent(
    ${name}
    INCLUDES
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/ssd1306
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/fonts
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/
    SOURCES
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/ssd1306/ssd1306.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/fonts/fonts.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/ssd1306_rpc.c
    LIBS
    system_config
    lib_debug
    os_core_api
    i2c_client
)

endfunction()