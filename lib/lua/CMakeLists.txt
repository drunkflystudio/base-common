
######################################################################################################################
## Lua Core

set(src
    _util.c
    lapi.c
    lapi.h
    lauxlib.c
    lauxlib.h
    lctype.c
    lctype.h
    ldebug.c
    ldebug.h
    ldo.c
    ldo.h
    lfunc.c
    lfunc.h
    lgc.c
    lgc.h
    ljumptab.h
    llimits.h
    lmem.c
    lmem.h
    lobject.c
    lobject.h
    lopcodes.c
    lopcodes.h
    lprefix.h
    lstate.c
    lstate.h
    lstring.c
    lstring.h
    ltable.c
    ltable.h
    ltm.c
    ltm.h
    lua.h
    luaconf.h
    lvm.c
    lvm.h
    lzio.c
    lzio.h
    )

source_group("Source Files" FILES ${src})

add_library(LuaCore STATIC ${src})
target_compile_definitions(LuaCore PUBLIC "$<$<CONFIG:Debug>:LUA_USE_APICHECK>")
target_include_directories(LuaCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(LuaCore PROPERTIES
    FOLDER "Libs/Lua"
    OUTPUT_NAME "luacore"
    )

######################################################################################################################
## Lua parser

set(src
    lauxlib.c
    lcode.c
    lcode.h
    llex.c
    llex.h
    lopnames.h
    lparser.c
    lparser.h
    )

source_group("Source Files" FILES ${src})

add_library(LuaParser STATIC ${src})
target_include_directories(LuaParser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(LuaParser PRIVATE LAUXLIB_FILE_IO)
target_link_libraries(LuaParser LuaCore)

set_target_properties(LuaParser PROPERTIES
    FOLDER "Libs/Lua"
    OUTPUT_NAME "luaparser"
    )

######################################################################################################################
## Lua bytecode

set(src
    ldump.c
    lundump.c
    lundump.h
    )

source_group("Source Files" FILES ${src})

add_library(LuaDump STATIC ${src})
target_compile_definitions(LuaDump PRIVATE LAPI_LUA_DUMP)
target_include_directories(LuaDump PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(LuaDump LuaCore)

set_target_properties(LuaDump PROPERTIES
    FOLDER "Libs/Lua"
    OUTPUT_NAME "luadump"
    )

######################################################################################################################
## Lua libraries

set(src
    linit.c
    lbaselib.c
    lcorolib.c
    ldblib.c
    liolib.c
    lmathlib.c
    #loadlib.c
    loslib.c
    lstrlib.c
    ltablib.c
    lutf8lib.c
    )

source_group("Source Files" FILES ${src})

add_library(LuaLibs STATIC ${src})
target_include_directories(LuaLibs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(LuaLibs LuaCore)

set_target_properties(LuaLibs PROPERTIES
    FOLDER "Libs/Lua"
    OUTPUT_NAME "lualibs"
    )

######################################################################################################################

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    target_link_libraries(LuaCore m)
    target_link_libraries(LuaLibs m)
endif()

######################################################################################################################
## Lua tools

if(NOT CMAKE_CROSSCOMPILING)
    set(LuaInterpreter LuaInterpreter)
    set(LuaCompiler LuaCompiler)
else()
    set(LuaInterpreter LuaInterpreterBuild)
    add_executable(LuaInterpreter IMPORTED GLOBAL)
    set_target_properties(LuaInterpreter PROPERTIES IMPORTED_LOCATION "${HOST_LUA}")
    set(LuaCompiler LuaCompilerBuild)
    add_executable(LuaCompiler IMPORTED GLOBAL)
    set_target_properties(LuaCompiler PROPERTIES IMPORTED_LOCATION "${HOST_LUAC}")
endif()

# Interpreter

set(src
    lua.c
    )

source_group("Source Files" FILES ${src})

add_executable(${LuaInterpreter} ${src})
target_link_libraries(${LuaInterpreter} LuaLibs LuaCore LuaParser LuaDump)
set_target_properties(${LuaInterpreter} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.."
    FOLDER "Tools/Lua"
    OUTPUT_NAME "lua"
    )

# Compiler

set(src
    luac.c
    )

source_group("Source Files" FILES ${src})

add_executable(${LuaCompiler} ${src})
target_link_libraries(${LuaCompiler} LuaCore LuaParser LuaDump)
set_target_properties(${LuaCompiler} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.."
    FOLDER "Tools/Lua"
    OUTPUT_NAME "luac"
    )
