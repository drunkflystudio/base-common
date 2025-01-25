#ifndef DRUNKFLY_COMMON_H
#define DRUNKFLY_COMMON_H

#include <pstdint.h>

#define bool int
#define false 0
#define true 1

#define UNUSED(X) ((void)(X))

#define STRUCT(X) \
    struct X; \
    typedef struct X X; \
    struct X

#define lua_State \
    struct lua_State

typedef unsigned int uint;

void* memAlloc(lua_State* L, size_t size);
void* memRealloc(lua_State* L, void* old, size_t oldSize, size_t newSize);
void memFree(lua_State* L, void* old, size_t oldSize);

#endif
