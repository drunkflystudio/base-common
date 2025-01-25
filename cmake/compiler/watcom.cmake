
add_compiler_flag(-w0)

macro(enable_warnings)
    add_compiler_flag_release(-we)
    add_compiler_flag(-w4)
endmacro()
