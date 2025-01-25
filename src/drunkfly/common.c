#include <drunkfly/common.h>
#include <lua.h>

void* memAlloc(lua_State* L, size_t size)
{
    void* ptr, *ud;
    lua_Alloc allocf = lua_getallocf(L, &ud);

    if (size == 0)
        size = 1;

    ptr = allocf(ud, NULL, 0, size);
    if (!ptr) {
        lua_pushliteral(L, "out of memory.");
        lua_error(L);
    }

    return ptr;
}

void* memRealloc(lua_State* L, void* old, size_t oldSize, size_t newSize)
{
    void* ptr, *ud;
    lua_Alloc allocf = lua_getallocf(L, &ud);

    if (!old)
        oldSize = 0;
    else if (oldSize == 0)
        oldSize = 1;

    if (newSize == 0)
        newSize = 1;

    ptr = allocf(ud, old, oldSize, newSize);
    if (!ptr) {
        lua_pushliteral(L, "out of memory.");
        lua_error(L);
    }

    return ptr;
}

void memFree(lua_State* L, void* old, size_t oldSize)
{
    lua_Alloc allocf;
    void* ud;

    if (!old)
        return;

    allocf = lua_getallocf(L, &ud);

    if (oldSize == 0)
        oldSize = 1;

    allocf(ud, old, oldSize, 0);
}
