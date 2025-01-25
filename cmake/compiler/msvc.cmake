
add_definitions(
    -D_CRT_SECURE_NO_DEPRECATE=1
    -D_CRT_SECURE_NO_WARNINGS=1
    -D_CRT_NONSTDC_NO_DEPRECATE=1
    -D_SCL_SECURE_NO_WARNINGS=1
    )

add_compiler_flag(/GS-)             # disable buffer security check
add_compiler_flag(/Gy)              # enable function-level linking

add_compiler_flag_release(/GL)      # whole program optimization
add_linker_flag_release(/LTCG)      # link-time code generation

remove_linker_flag("/INCREMENTAL(:NO)?")
add_linker_flag_release(/INCREMENTAL:NO)

add_linker_flag_release(/OPT:REF)
add_linker_flag_release(/OPT:ICF=20)

macro(enable_warnings)
    add_compiler_flag_release(/WX)  # treat warnings as errors
    add_compiler_flag(/W4)          # warning level 4
    add_compiler_flag(/Wall)        # enable all warnings
    add_compiler_flag(/wd4710)      # 4710: function not inlined
    add_compiler_flag(/wd4711)      # 4711: function selected for automatic inline expansion
    add_compiler_flag(/wd4820)      # 4820: padding added after data member
    add_compiler_flag(/wd5045)      # 5045: compiler will insert Spectre mitigation if /Qspectre switch specified
endmacro()
