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

struct lua_State;

#define lua_State \
    struct lua_State

typedef unsigned int uint;

#endif
