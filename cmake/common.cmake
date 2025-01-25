if(COMMON_CMAKE_INCLUDED)
    return()
else()
    set(COMMON_CMAKE_INCLUDED TRUE)
endif()

######################################################################################################################

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

######################################################################################################################

get_filename_component(ROOT_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
get_filename_component(ROOT_DIR "${ROOT_DIR}" DIRECTORY)

get_filename_component(BINARY_DIR "${CMAKE_BINARY_DIR}" DIRECTORY)

######################################################################################################################

include_directories("${ROOT_DIR}/lib/pstdint")

######################################################################################################################

include("${ROOT_DIR}/cmake/flags.cmake")

######################################################################################################################

if(MSVC)
    include("${ROOT_DIR}/cmake/compiler/msvc.cmake")
elseif(BORLAND)
    include("${ROOT_DIR}/cmake/compiler/borland.cmake")
elseif(WATCOM)
    include("${ROOT_DIR}/cmake/compiler/watcom.cmake")
elseif(EMSCRIPTEN)
    include("${ROOT_DIR}/cmake/compiler/emscripten.cmake")
else()
    include("${ROOT_DIR}/cmake/compiler/gnu.cmake")
endif()

######################################################################################################################

macro(maybe_write_file file contents)
    set(do_write TRUE)
    if(EXISTS "${file}")
        file(READ "${file}" old)
        if("${old}" STREQUAL "${contents}")
            set(do_write FALSE)
        endif()
    endif()
    if(do_write)
        file(WRITE "${file}" "${contents}")
    endif()
endmacro()

######################################################################################################################

add_subdirectory("${ROOT_DIR}/lib" common_lib)
add_subdirectory("${ROOT_DIR}/tests" common_tests)

######################################################################################################################
## Run base tests

if(NOT CMAKE_CROSSCOMPILING OR (OLD_BORLAND AND WIN32) OR (OLD_WATCOM AND WIN32))

set(tests_run "${CMAKE_CURRENT_BINARY_DIR}/t_base.run")
source_group("Generated Files" FILES "${tests_run}")

add_custom_command(OUTPUT "${tests_run}"
    COMMAND BaseTests "${tests_run}"
    DEPENDS BaseTests
    COMMENT "Running base tests"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )

add_custom_target(RunBaseTests
    DEPENDS "${tests_run}"
    SOURCES "${tests_run}"
    )

else()
add_custom_target(RunBaseTests)
endif()

set_target_properties(RunBaseTests PROPERTIES FOLDER "CMake")

######################################################################################################################

add_subdirectory("${ROOT_DIR}/src" common_src)
