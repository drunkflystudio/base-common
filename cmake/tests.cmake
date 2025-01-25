
macro(register_test_runner name file)
    if(NOT CMAKE_CROSSCOMPILING OR (OLD_BORLAND AND WIN32) OR (OLD_WATCOM AND WIN32))
        set(tests_run "${CMAKE_CURRENT_BINARY_DIR}/${file}.run")
        source_group("Generated Files" FILES "${tests_run}")

        add_custom_command(OUTPUT "${tests_run}"
            COMMAND "${name}" "${tests_run}"
            DEPENDS "${name}"
            COMMENT "Running ${name}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            )

        add_custom_target(Run${name}
            DEPENDS "${tests_run}"
            SOURCES "${tests_run}"
            )
    else()
        add_custom_target(Run${name})
    endif()
    set_target_properties(Run${name} PROPERTIES FOLDER "CMake")
endmacro()
