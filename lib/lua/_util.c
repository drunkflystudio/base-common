#include "lua.h"
#include "lauxlib.h"
#include "ldebug.h"
#include <stdio.h>
#include <stdlib.h>

const char luastr_hex[] = "-0123456789abcdef";
const char luastr_name[] = "__name";
const char luastr_gc[] = "__gc";
const char luastr_len[] = "__len";
const char luastr_to_string[] = "__tostring";
const char luastr___index[] = "__index";
const char luastr_new_index[] = "__newindex";
const char luastr_close[] = "__close";
const char luastr___call[] = "__call";
const char luastr_upvalue[] = "upvalue";
const char luastr_function[] = "function";
const char luastr_number[] = "number";
const char luastr_metamethod[] = "metamethod";
const char luastr_light_userdata[] = "light userdata";
const char luastr_invalid_option[] = "invalid option";
const char luastr_nil[] = "nil";
const char luastr_false[] = "false";
const char luastr_true[] = "true";
const char luastr_c_stack_overflow[] = "C stack overflow";
const char luastr_err_no_str[] = "error object is not a string";
const char luastr_attempt_to[] = "attempt to";
const char luastr_no_memory[18] = "not enough memory";
const char luastr_number_no_integer[] = "number has no integer representation";
const char luastr_equal_question[3] = "=?";

int lua_pointer2str(lua_State* L, char* dst, int sz, const void* p)
{
    uintptr_t u = (uintptr_t)p;
    const int steps = (sizeof(void*) * CHAR_BIT) / 4;
    int i;

    if (sz < 3 + steps)
        luaG_runerror(L, "%s: buffer is too small", "lua_pointer2str");

    *dst++ = '0';
    *dst++ = 'x';
    dst += steps;
    *dst = 0;
    for (i = 0; i < steps; i++) {
        *--dst = luastr_hex[1 + (u & 15)];
        u >>= 4;
    }

    return 2 + steps;
}

int lua_integer2str(lua_State* L, char* dst, int sz, lua_Integer value)
{
    char* start;
    int n = 0;

    if (value < 0) {
        ++n;
        if (n > sz)
            goto toolarge;
        *dst++ = '-';
        value = -value;
    }

    start = dst;
    do {
        ++n;
        if (n > sz)
            goto toolarge;
        *dst++ = luastr_hex[1 + (value % 10)];
        value /= 10;
    } while (value != 0);

    if (n + 1 > sz) {
      toolarge:
        luaG_runerror(L, "%s: buffer is too small", "lua_integer2str");
    }

    *dst = 0;

    while (start < --dst) {
        char ch = *dst;
        *dst = *start;
        *start++ = ch;
    }

    return n;
}

int lua_number2str(char* dst, int sz, double n)
{
    return sprintf(dst, LUA_NUMBER_FMT, n);
}

lua_Number lua_str2number(const char* str, char** endptr)
{
    return strtod(str, endptr);
}
