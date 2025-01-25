
set(clang "${CMAKE_CURRENT_LIST_DIR}/../../tools/clang350_linux64")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER "${clang}/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${clang}/bin/clang++.exe")
set(CMAKE_AR "${clang}/bin/x86_64-unknown-linux-gnu-ar.exe")

set(flags "-target x86_64-unknown-linux-gnu -static \"-B${clang}/usr/lib64\" -isystem \"${clang}/usr/include\"")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} ${flags} -D_ISOC99_SOURCE")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} ${flags}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} \"-Wl,-L${clang}/usr/lib64\"")

set(CMAKE_FIND_ROOT_PATH "${clang}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
