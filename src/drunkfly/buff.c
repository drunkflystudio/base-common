#include <drunkfly/buff.h>
#include <string.h>
#include <lauxlib.h>
#include <lobject.h>

void buffInit(Buff* buff, lua_State* L)
{
    luaL_buffinitsize(L, (luaL_Buffer*)&buff->buffer, LUAL_BUFFERSIZE+1);
}

void buffAppend(Buff* buff, const void* data, size_t size)
{
    if (size > 0) {
        char *b = prepbuffsize((luaL_Buffer*)&buff->buffer, size, 0);
        memcpy(b, data, size);
        luaL_addsize(&buff->buffer, size);
    }
}

void buffPrintC(Buff* buff, char ch)
{
    if (buff->buffer.n >= buff->buffer.size)
        prepbuffsize((luaL_Buffer*)&buff->buffer, 1, 0);
    buff->buffer.b[buff->buffer.n++] = ch;
}

void buffPrintUtf8(Buff* buff, uint32_t ch)
{
    /* FIXME */
    buffPrintC(buff, (char)ch);
}

void buffPrintS(Buff* buff, const char* str)
{
    buffAppend(buff, str, strlen(str));
}

void buffPrintV(Buff* buff, const char* fmt, va_list args)
{
    const char* str = luaO_pushvfstring(buff->buffer.L, fmt, args);
    buffAppend(buff, str, strlen(str));
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
    lua_State* L = buff->buffer.L;
    if (outSize)
        *outSize = buff->buffer.n;
    return lua_pushlstring(L, buff->buffer.b, buff->buffer.n);
}
