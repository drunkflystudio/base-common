#include <drunkfly/vm.h>
#include <lua.h>
#include <lauxlib.h>
#include <lstate.h>
#include <string.h>
#include <setjmp.h>

STRUCT(VMStartupContext) {
    char tempBuffer[1024];
    void* initData;
    VMINITPROC init;
    VMMAINPROC entry;
    VMLOGGERPROC logger;
    jmp_buf crash;
};

static const char g_ErrorNotString[] = "error object is not a string";
static const char g_PanicPrefix[] = "unhandled exception: ";

static void vmCrash(lua_State* L)
{
    VMStartupContext* ctx = (VMStartupContext*)(G(L)->ud);
    longjmp(ctx->crash, 1);
}

static int vmMain(lua_State* L)
{
    VMStartupContext* ctx = (VMStartupContext*)(G(L)->ud);

    if (ctx->init) {
        lua_pushcfunction(L, ctx->init);
        lua_call(L, 0, 0);
    }

    lua_gc(L, LUA_GCRESTART);
    lua_gc(L, LUA_GCGEN, 0, 0);

    if (ctx->entry) {
        lua_pushcfunction(L, ctx->entry);
        lua_call(L, 0, 0);
    }

    return 0;
}

static int vmPanic(lua_State* L)
{
    VMStartupContext* ctx;
    const char* msg;

    ctx = (VMStartupContext*)(G(L)->ud);
    msg = (lua_type(L, -1) == LUA_TSTRING ? lua_tostring(L, -1) : g_ErrorNotString);

    if (ctx->logger) {
        size_t len1 = sizeof(g_PanicPrefix) - 1;
        size_t len2 = strlen(msg);
        if (len1 + len2 >= sizeof(ctx->tempBuffer))
            len2 = sizeof(ctx->tempBuffer) - len1 - 1;
        memcpy(ctx->tempBuffer, g_PanicPrefix, len1);
        memcpy(ctx->tempBuffer + len1, msg, len2);
        ctx->tempBuffer[len1 + len2] = 0;
        ctx->logger(VM_FATAL, ctx->tempBuffer);
    }

    vmCrash(L);
    return 0;
}

void* vmGetInitData(lua_State* L)
{
    VMStartupContext* ctx = (VMStartupContext*)(G(L)->ud);
    return ctx->initData;
}

void* vmAlloc(lua_State* L, size_t size)
{
    void* ptr, *ud;
    lua_Alloc allocf = lua_getallocf(L, &ud);

    if (size == 0)
        size = 1;

    ptr = allocf(ud, NULL, 0, size);
    if (!ptr) {
        lua_pushliteral(L, "not enough memory");
        lua_error(L);
    }

    return ptr;
}

void* vmRealloc(lua_State* L, void* old, size_t oldSize, size_t newSize)
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
        lua_pushliteral(L, "not enough memory");
        lua_error(L);
    }

    return ptr;
}

void vmFree(lua_State* L, void* old, size_t oldSize)
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

int vmCheckError(lua_State* L, int status)
{
    if (status != LUA_OK) {
        VMStartupContext* ctx = (VMStartupContext*)(G(L)->ud);

        const char* msg = lua_tostring(L, -1);
        if (!msg) {
            lua_pop(L, 1);
            msg = lua_pushfstring(L, "(%s)", g_ErrorNotString);
        }

        if (ctx->logger) {
            const char* fmt = lua_pushfstring(L, "VM error:\n%s", msg);
            ctx->logger(VM_ERROR, fmt);
            lua_pop(L, 1);
        }

        lua_pop(L, 1);
    }

    return status;
}

static int vmErrorHandler(lua_State* L)
{
    const char* msg = lua_tostring(L, 1);
    if (!msg) {
        if (luaL_callmeta(L, 1, "__tostring") && lua_type(L, -1) == LUA_TSTRING)
            return 1;
        else
            msg = lua_pushfstring(L, "(error object is a %s value)", luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);
    return 1;
}

int vmProtectedCall(lua_State* L, int nargs, int nret)
{
    int errorHandler = lua_gettop(L) - nargs;
    int ret;

    lua_pushcfunction(L, vmErrorHandler);
    lua_insert(L, errorHandler);
    ret = lua_pcall(L, nargs, nret, errorHandler);
    lua_remove(L, errorHandler);

    return ret;
}

static void vmStopHook(lua_State* L, lua_Debug* ar)
{
    UNUSED(ar);
    lua_sethook(L, NULL, 0, 0);
    lua_pushliteral(L, "interrupted!");
    lua_error(L);
}

void vmInterrupt(lua_State* L)
{
    const int flag = LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE | LUA_MASKCOUNT;
    lua_sethook(L, vmStopHook, flag, 1);
}

bool vmRun(VMINITPROC init, VMMAINPROC entry, VMLOGGERPROC logger, void* initData)
{
    VMStartupContext ctx;
    lua_State* volatile L;
    int status;

    L = luaL_newstate();
    if (!L) {
        if (logger)
            logger(VM_FATAL, "unable to initialize VM.");
        return false;
    }

    ctx.initData = initData;
    ctx.init = init;
    ctx.entry = entry;
    ctx.logger = logger;

    if (setjmp(ctx.crash) != 0) {
        lua_close(L);
        return false;
    }

    G(L)->ud = &ctx;
    lua_atpanic(L, vmPanic);

    lua_gc(L, LUA_GCSTOP);

    lua_pushcfunction(L, vmMain);
    status = vmProtectedCall(L, 0, 0);
    vmCheckError(L, status);

    lua_close(L);

    return (status == LUA_OK);
}
