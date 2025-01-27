#include <drunkfly/vm.h>
#include <assert.h>
#include <lua.h>
#include <lauxlib.h>
#include <lstate.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>

#ifdef _MSC_VER
#pragma warning(disable:4702) /* unreachable code */
#endif

STRUCT(VMStartupContext) {
    char tempBuffer[1024];
    void* initData;
    VMINITPROC init;
    VMMAINPROC entry;
    VMLOGGERPROC logger;
    jmp_buf crash;
    size_t tempBufferPosition;
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

    luaL_checkversion(L);

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

STRUCT(MemBlock) {
    size_t size;
    char payload[1];
};

static void* vmAllocator(void* ud, void* ptr, size_t osize, size_t nsize)
{
  #ifdef NDEBUG
    UNUSED(osize);
    UNUSED(ud);
    if (nsize != 0)
        return realloc(ptr, nsize);
    else {
        free(ptr);
        return NULL;
    }
  #else
    MemBlock* mb = NULL;
    UNUSED(ud);

    if (ptr == NULL) {
        assert(osize < 16);  /* zero or tag */
    } else {
        assert(osize != 0);
        mb = (MemBlock*)((char*)ptr - offsetof(MemBlock, payload));
        assert(mb->size == osize);
    }

    if (nsize != 0) {
        mb = (MemBlock*)realloc(mb, offsetof(MemBlock, payload) + nsize);
        if (mb)
            mb->size = nsize;
        return mb->payload;
    } else {
        if (osize == 0)
            assert(mb == NULL);
        else {
            assert(mb != NULL);
            free(mb);
        }
        return NULL;
    }
  #endif
}

static void vmTempBufferFlush(VMStartupContext* ctx)
{
    ctx->tempBuffer[ctx->tempBufferPosition] = 0;
    ctx->tempBufferPosition = 0;
    if (ctx->logger)
        ctx->logger(VM_WARN, ctx->tempBuffer);
}

static void vmTempBufferAppend(VMStartupContext* ctx, const char* message)
{
    while (*message) {
        if (ctx->tempBufferPosition >= sizeof(ctx->tempBuffer) - 1)
            vmTempBufferFlush(ctx);
        ctx->tempBuffer[ctx->tempBufferPosition++] = *message++;
    }
}

static void vmWarning(void* ud, const char* message, int tocont);

static void vmWarningContinue(void* ud, const char* message, int tocont)
{
    lua_State* L = (lua_State*)ud;
    VMStartupContext* ctx = (VMStartupContext*)(G(L)->ud);
    vmTempBufferAppend(ctx, message);
    if (tocont)
        lua_setwarnf(L, vmWarningContinue, L);
    else {
        vmTempBufferFlush(ctx);
        lua_setwarnf(L, vmWarning, L);
    }
}

static void vmWarning(void* ud, const char* message, int tocont)
{
    lua_State* L = (lua_State*)ud;
    VMStartupContext* ctx = (VMStartupContext*)(G(L)->ud);
    ctx->tempBufferPosition = 0;
    vmWarningContinue(ud, message, tocont);
}

bool vmRun(VMINITPROC init, VMMAINPROC entry, VMLOGGERPROC logger, void* initData)
{
    lua_State* volatile L = NULL;
    VMStartupContext ctx;
    int status;

    ctx.initData = initData;
    ctx.init = init;
    ctx.entry = entry;
    ctx.logger = logger;
    ctx.tempBufferPosition = 0;

    if (setjmp(ctx.crash) != 0) {
        if (L)
            lua_close(L);
        return false;
    }

    L = lua_newstate(vmAllocator, &ctx);
    if (!L) {
        if (logger)
            logger(VM_FATAL, "unable to initialize VM.");
        return false;
    }

    assert(G(L)->ud == &ctx);
    lua_atpanic(L, vmPanic);
    lua_setwarnf(L, vmWarning, L);

    lua_gc(L, LUA_GCSTOP);

    lua_pushcfunction(L, vmMain);
    status = vmProtectedCall(L, 0, 0);
    vmCheckError(L, status);

    lua_close(L);

    return (status == LUA_OK);
}
