
######################################################################################################################
# GnuWin32

if(WIN32)
    set(GREP "${TOOLS_DIR}/gnuwin32/grep.exe")
    set(DIFF "${TOOLS_DIR}/gnuwin32/diff.exe")
    macro(require_gnu_tools)
        external_git_repo("${TOOLS_DIR}/gnuwin32" "https://github.com/thirdpartystuff/win32-gnu")
    endmacro()
else()
    set(GREP "grep")
    set(DIFF "diff")
    macro(require_gnu_tools)
    endmacro()
endif()

######################################################################################################################
# Ninja

if(WIN32)
    macro(require_ninja)
        external_git_repo("${TOOLS_DIR}/ninja" "https://github.com/thirdpartystuff/win32-ninja")
        add_PATH("${TOOLS_DIR}/ninja")
    endmacro()
else()
    macro(require_ninja)
        # FIXME
    endmacro()
endif()

######################################################################################################################
# MinGW make

macro(require_mingw32_make)
    external_git_repo("${TOOLS_DIR}/mingw32_make" "https://github.com/thirdpartystuff/win32-mingw32-make")
    if(WIN32)
        add_PATH("${TOOLS_DIR}/mingw32_make")
    endif()
endmacro()

######################################################################################################################
# Python 3

if(WIN32)
    macro(require_python3)
        external_git_repo("${TOOLS_DIR}/python3" "https://github.com/thirdpartystuff/win32-python3")
        add_PATH("${TOOLS_DIR}/python3")
    endmacro()
else()
    macro(require_python3)
        # FIXME
    endmacro()
endif()

######################################################################################################################
# Emscripten

macro(require_emscripten version)
    require_python3()
    external_git_repo("${TOOLS_DIR}/emsdk" "https://github.com/emscripten-core/emsdk.git")
    do(emsdk.bat install "${version}" WORKING_DIRECTORY "${TOOLS_DIR}/emsdk")
    do(emsdk.bat activate "${version}" WORKING_DIRECTORY "${TOOLS_DIR}/emsdk")
    do(emsdk_env.bat WORKING_DIRECTORY "${TOOLS_DIR}/emsdk" ERROR_VARIABLE emsdk_env)
    string(REGEX MATCH "EMSDK = [^\r\n]*" EMSDK "${emsdk_env}")
    string(SUBSTRING "${EMSDK}" 8 -1 EMSDK)
    string(REGEX MATCH "EMSDK_PYTHON = ([^\r\n]*)" EMSDK_PYTHON "${emsdk_env}")
    string(SUBSTRING "${EMSDK_PYTHON}" 15 -1 EMSDK_PYTHON)
    string(REGEX MATCH "EMSDK_NODE = ([^\r\n]*)" EMSDK_NODE "${emsdk_env}")
    string(SUBSTRING "${EMSDK_NODE}" 13 -1 EMSDK_NODE)
    message(STATUS "EMSDK = \"${EMSDK}\"")
    message(STATUS "EMSDK_PYTHON = \"${EMSDK_PYTHON}\"")
    message(STATUS "EMSDK_NODE = \"${EMSDK_NODE}\"")
    set(ENV{EMSDK} "${EMSDK}")
    set(ENV{EMSDK_PYTHON} "${EMSDK_PYTHON}")
    set(ENV{EMSDK_NODE} "${EMSDK_NODE}")
    set(EMSCRIPTEN "${TOOLS_DIR}/emsdk/upstream/emscripten")
    add_PATH("${EMSDK}")
    add_PATH("${EMSCRIPTEN}")
endmacro()

######################################################################################################################
# Borland C++ 4.52

macro(require_borland_4_5)
    external_git_repo("${TOOLS_DIR}/borland45" "https://github.com/thirdpartystuff/win32-borland452")
    if(WIN32)
        add_PATH("${TOOLS_DIR}/borland45/bin")
    endif()
endmacro()

######################################################################################################################
# Watcom 10.0a

macro(require_watcom10)
    string(LENGTH "${TOOLS_DIR}/watcom10" length)
    if(length GREATER 53)
        message(FATAL_ERROR "\n=== Watcom 10.0a is unable to handle such long path: ===\n\"${TOOLS_DIR}/watcom10\"\n")
    endif()
    external_git_repo("${TOOLS_DIR}/watcom10" "https://github.com/thirdpartystuff/win32-watcom10")
    if(WIN32)
        add_PATH("${TOOLS_DIR}/watcom10/binnt")
    endif()
endmacro()

######################################################################################################################
# MinGW 8.10 32-bit

macro(require_mingw810_32)
    external_git_repo("${TOOLS_DIR}/mingw810_32" "https://github.com/thirdpartystuff/win32-mingw810-32")
    if(WIN32)
        add_PATH("${TOOLS_DIR}/mingw810_32/bin")
    endif()
endmacro()

######################################################################################################################
# MinGW 8.10 64-bit

macro(require_mingw810_64)
    external_git_repo("${TOOLS_DIR}/mingw810_64" "https://github.com/thirdpartystuff/win32-mingw810-64")
    if(WIN32)
        add_PATH("${TOOLS_DIR}/mingw810_64/bin")
    endif()
endmacro()

######################################################################################################################
# Clang 3.5.0 targetting Linux 64-bit from Win32

macro(require_clang350_linux64)
    external_git_repo("${TOOLS_DIR}/clang350_linux64" "https://github.com/thirdpartystuff/win32-clang350-linux64")
    if(WIN32)
        add_PATH("${TOOLS_DIR}/clang350_linux64/bin")
    endif()
endmacro()
