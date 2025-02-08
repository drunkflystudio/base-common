#ifndef DRUNKFLY_BUFF_H
#define DRUNKFLY_BUFF_H

#include <drunkfly/common.h>
#include <stdarg.h>
#include <lauxlib.h>

STRUCT(Buff) {
    luaL_Buffer buffer;
};

void buffInit(Buff* buff, lua_State* L);
void buffPrintC(Buff* buff, char ch);
void buffPrintUtf8(Buff* buff, uint32_t ch);
void buffPrintS(Buff* buff, const char* str);
void buffPrintV(Buff* buff, const char* fmt, va_list args);
void buffPrintF(Buff* buff, const char* fmt, ...);
const char* buffEnd(Buff* buff, size_t* outSize);

#endif
