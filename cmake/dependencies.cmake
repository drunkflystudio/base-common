# This file is executed by buildtool before building the project
# Use it to setup the environment and compile additional tools if needed

if(CROSSCOMPILING)
    build_host_tool(lua
        DIRECTORY "${CURRENT_PROJECT_DIR}"
        TARGETS LuaInterpreter LuaCompiler
        EXECUTABLES lua luac
        )
endif()
