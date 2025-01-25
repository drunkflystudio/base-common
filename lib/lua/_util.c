#include "lua.h"
#include "lauxlib.h"
#include "ldebug.h"
#include <stdio.h>
#include <stdlib.h>

static const char hex[] = "0123456789abcdef";

int lua_pointer2str(lua_State* L, char* dst, int sz, const void* p)
{
    uintptr_t u = (uintptr_t)p;
    const int steps = (sizeof(void*) * CHAR_BIT) / 4;
    int i;

    if (sz < 3 + steps)
        luaG_runerror(L, "lua_pointer2str: buffer is too small.");

    *dst++ = '0';
    *dst++ = 'x';
    dst += steps;
    *dst = 0;
    for (i = 0; i < steps; i++) {
        *--dst = hex[u & 15];
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
        *dst++ = hex[value % 10];
        value /= 10;
    } while (value != 0);

    if (n + 1 > sz) {
      toolarge:
        luaG_runerror(L, "lua_integer2str: buffer is too small.");
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
