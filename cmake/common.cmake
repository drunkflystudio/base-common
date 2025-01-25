
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

get_filename_component(ROOT_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
get_filename_component(ROOT_DIR "${ROOT_DIR}" DIRECTORY)

get_filename_component(BINARY_DIR "${CMAKE_BINARY_DIR}" DIRECTORY)

include_directories("${ROOT_DIR}/lib/pstdint")

include("${ROOT_DIR}/cmake/flags.cmake")

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
