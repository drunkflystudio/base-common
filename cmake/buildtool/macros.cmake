
######################################################################################################################

macro(do)
    set(status_code -1)
    execute_process(
        COMMAND ${ARGN}
        RESULT_VARIABLE status_code
        )
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "\n======> Command failed with exit code ${status_code}! <======\n\n")
    endif()
endmacro()

######################################################################################################################

macro(external_git_repo dir repo)
    if(NOT EXISTS "${dir}/.git")
        do(git clone "${repo}" "${dir}")
    endif()
endmacro()

######################################################################################################################

macro(require_win32_host what)
    if(NOT WIN32)
        message(FATAL_ERROR "${what} requires Win32 host.")
    endif()
endmacro()

######################################################################################################################

macro(add_PATH path)
    if(WIN32)
        set(ENV{PATH} "${path};$ENV{PATH}")
    else()
        set(ENV{PATH} "${path}:$ENV{PATH}")
    endif()
endmacro()

######################################################################################################################

macro(get_build_types var in)
    set(t_debug FALSE)
    set(t_release FALSE)
    set(t_relwithdebinfo FALSE)
    set(t_minsizerel FALSE)
    foreach(type ${in})
        if("${type}" STREQUAL "debug")
            set(t_debug TRUE)
        elseif("${type}" STREQUAL "release")
            set(t_release TRUE)
        elseif("${type}" STREQUAL "relwithdebinfo")
            set(t_relwithdebinfo TRUE)
        elseif("${type}" STREQUAL "minsizerel")
            set(t_minsizerel TRUE)
        else()
            message(FATAL_ERROR "Invalid build type \"${type}\".")
        endif()
    endforeach()
    if(NOT t_debug AND NOT t_release AND NOT t_relwithdebinfo AND NOT t_minsizerel)
        set(out "Release")
    else()
        set(out)
        if(t_debug)
            list(APPEND out Debug)
        endif()
        if(t_release)
            list(APPEND out Release)
        endif()
        if(t_relwithdebinfo)
            list(APPEND out RelWithDebInfo)
        endif()
        if(t_minsizerel)
            list(APPEND out MinSizeRel)
        endif()
    endif()
    set(${var} ${out})
endmacro()

######################################################################################################################

macro(generate_project)
    set(options)
    set(one DIRECTORY EXECUTABLE GENERATOR TOOLCHAIN BUILD_TYPE)
    set(multi)
    cmake_parse_arguments(gp "${options}" "${one}" "${multi}" ${ARGN})

    if(NOT gp_DIRECTORY)
        message(FATAL_ERROR "generate_project: directory not specified!")
    endif()
    if(NOT gp_EXECUTABLE)
        message(FATAL_ERROR "generate_project: executable not specified!")
    endif()

    set(exe_exists FALSE)
    if(NOT gp_EXECUTABLE MATCHES "%CFG%")
        if(EXISTS "${BUILD_DIR}/${gp_DIRECTORY}/${gp_EXECUTABLE}")
            set(exe_exists TRUE)
        endif()
    else()
        foreach(bt Debug Release RelWithDebInfo MinSizeRel)
            string(REPLACE "%CFG%" "${bt}" exe "${gp_EXECUTABLE}")
            if(EXISTS "${BUILD_DIR}/${gp_DIRECTORY}/${exe}")
                set(exe_exists TRUE)
                break()
            endif()
        endforeach()
    endif()

    if(NOT exe_exists)
        if(NOT EXISTS "${BUILD_DIR}/${gp_DIRECTORY}/cmake")
            file(MAKE_DIRECTORY "${BUILD_DIR}/${gp_DIRECTORY}/cmake")
        endif()

        set(args)

        if(gp_GENERATOR)
            if(NOT "${gp_GENERATOR}" STREQUAL "Makefiles")
                set(generator "${gp_GENERATOR}")
            else()
                if(NOT WIN32)
                    set(generator "Unix Makefiles")
                else()
                    set(generator "MinGW Makefiles")
                    require_mingw32_make()
                    list(APPEND args "-DCMAKE_MAKE_PROGRAM=${TOOLS_DIR}/mingw32_make/mingw32-make.exe")
                endif()
            endif()
        else()
            set(generator Ninja)
        endif()
        list(APPEND args -G "${generator}")
        if("${generator}" STREQUAL "Ninja")
            require_ninja()
            if(WIN32)
                list(APPEND args "-DCMAKE_MAKE_PROGRAM=${TOOLS_DIR}/ninja/ninja.exe")
            endif()
        endif()

        if(gp_ARCHITECTURE)
            list(APPEND args -A "${gp_ARCHITECTURE}")
        endif()

        if(gp_TOOLCHAIN)
            if(EXISTS "${gp_TOOLCHAIN}")
                set(path "${gp_TOOLCHAIN}")
            else()
                set(path "${CMAKE_DIR}/toolchain/${gp_TOOLCHAIN}.cmake")
            endif()
            list(APPEND args "-DCMAKE_TOOLCHAIN_FILE=${path}")
        endif()

        if(gp_BUILD_TYPE)
            list(APPEND args "-DCMAKE_BUILD_TYPE=${gp_BUILD_TYPE}")
        endif()

        do("${CMAKE_COMMAND}" ${args} "${PROJECT_DIR}"
            WORKING_DIRECTORY "${BUILD_DIR}/${gp_DIRECTORY}/cmake")
    endif()
endmacro()

######################################################################################################################

macro(build_project)
    set(options)
    set(one DIRECTORY)
    set(multi)
    cmake_parse_arguments(gp "${options}" "${one}" "${multi}" ${ARGN})

    if(NOT gp_DIRECTORY)
        message(FATAL_ERROR "build_project: directory not specified!")
    endif()

    set(args)
    if(gp_BUILD_TYPE)
        list(APPEND args --config "${gp_BUILD_TYPE}")
    endif()
    if(gp_PARALLEL)
        list(APPEND args --parallel)
    endif()

    do("${CMAKE_COMMAND}" --build . ${args}
        WORKING_DIRECTORY "${BUILD_DIR}/${gp_DIRECTORY}/cmake")
endmacro()
