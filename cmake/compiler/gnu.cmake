
add_compiler_flag(-fno-ident)
add_compiler_flag(-fvisibility=hidden)
add_compiler_flag(-fdata-sections)
add_compiler_flag(-ffunction-sections)

if(CMAKE_COMPILER_IS_GNUCC)
    add_compiler_flag(-fno-plt)
endif()

add_linker_flag_release(-O)
add_linker_flag_release_only(-s)
add_linker_flag_release_only(-static-libgcc)

macro(enable_warnings)
    add_compiler_flag(-Werror)
    add_compiler_flag(-ansi)
    add_compiler_flag(-pedantic)
    add_compiler_flag(-Wall)
    add_compiler_flag(-Wextra)
    add_compiler_flag(-Wshadow)
    add_compiler_flag(-Wconversion)
endmacro()
