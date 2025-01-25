#ifndef DRUNKFLY_VM_H
#define DRUNKFLY_VM_H

#include <drunkfly/common.h>

typedef enum VMMSGTYPE {
    VM_ERROR,
    VM_FATAL
} VMMSGTYPE;

typedef int (*VMINITPROC)(lua_State* L);
typedef int (*VMMAINPROC)(lua_State* L);
typedef int (*VMLOGGERPROC)(VMMSGTYPE type, const char* message);

void* vmGetInitData(lua_State* L);

void* vmAlloc(lua_State* L, size_t size);
void* vmRealloc(lua_State* L, void* old, size_t oldSize, size_t newSize);
void vmFree(lua_State* L, void* old, size_t oldSize);

int vmProtectedCall(lua_State* L, int nargs, int nret);

void vmInterrupt(lua_State* L);
bool vmRun(VMINITPROC init, VMMAINPROC entry, VMLOGGERPROC logger, void* initData);

#endif
