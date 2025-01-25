
add_subdirectory(tests)

######################################################################################################################
## Run tests

if(NOT CMAKE_CROSSCOMPILING OR (OLD_BORLAND AND WIN32) OR (OLD_WATCOM AND WIN32))

set(tests_run "${CMAKE_CURRENT_BINARY_DIR}/tests.run")
source_group("Generated Files" FILES "${tests_run}")

add_custom_command(OUTPUT "${tests_run}"
    COMMAND CompilerTests "${tests_run}"
    DEPENDS CompilerTests
    COMMENT "Running base tests"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )

endif()
