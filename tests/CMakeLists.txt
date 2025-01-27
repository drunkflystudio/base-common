
enable_warnings()

set(src
    ../lib/pstdint/pstdint.h
    main.c
    pstdint.c
    vm.c
    )

source_group("Source Files" FILES ${src})
set_source_files_properties(pstdint.c PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../lib/pstdint/pstdint.h")

add_executable(BaseTests ${src})
target_link_libraries(BaseTests PRIVATE FlyCommon)
set_target_properties(BaseTests PROPERTIES
    FOLDER "Tests"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.."
    OUTPUT_NAME "t_base"
    )
