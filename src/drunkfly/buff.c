#include <drunkfly/buff.h>
#include <lauxlib.h>
#include <lobject.h>

void buffInit(Buff* buff, lua_State* L)
{
    luaL_buffinit(L, &buff->buffer);
}

void buffPrintC(Buff* buff, char ch)
{
    luaL_addchar(&buff->buffer, ch);
}

void buffPrintUtf8(Buff* buff, uint32_t ch)
{
    /* FIXME */
    luaL_addchar(&buff->buffer, (char)ch);
}

void buffPrintS(Buff* buff, const char* str)
{
    luaL_addstring(&buff->buffer, str);
}

void buffPrintV(Buff* buff, const char* fmt, va_list args)
{
    luaO_pushvfstring(buff->buffer.L, fmt, args);
    luaL_addvalue(&buff->buffer);
}

void buffPrintF(Buff* buff, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    buffPrintV(buff, fmt, args);
    va_end(args);
}

const char* buffEnd(Buff* buff, size_t* outSize)
{
    luaL_pushresult(&buff->buffer);
    return lua_tolstring(buff->buffer.L, -1, outSize);
}
