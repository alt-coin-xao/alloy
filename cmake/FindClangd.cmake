SET(Clangd_FOUND TRUE)

MACRO(ADDCLANGDLSP)

  MESSAGE( STATUS "Export for LSP, Clangd" )

  add_custom_target(clangdlsp ALL
    DEPENDS compile_commands.json
    )

  add_custom_command( 
    OUTPUT compile_commands.json
    COMMAND ${CMAKE_COMMAND} -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    )

ENDMACRO(ADDCLANGDLSP)
