
enable_warnings()

set(src
    drunkfly/common.h
    drunkfly/vm.c
    drunkfly/vm.h
    )

source_group("Source Files\\drunkfly" FILES ${src})
add_library(FlyCommon STATIC ${src})

set_target_properties(FlyCommon PROPERTIES FOLDER "Libs")
target_include_directories(FlyCommon PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(FlyCommon PUBLIC LuaCore)
