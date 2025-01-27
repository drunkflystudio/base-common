#include <drunkfly/vm.h>
#include <stdio.h>

static bool initCalled;
static bool runCalled;
static bool messagePrinted;

static int initTests(lua_State* L)
{
    UNUSED(L);
    initCalled = true;
    return 0;
}

static int runTests(lua_State* L)
{
    UNUSED(L);
    runCalled = true;
    return 0;
}

static void printMessage(VMMSGTYPE type, const char* message)
{
    UNUSED(type);
    fprintf(stderr, "%s\n", message);
    messagePrinted = true;
}

bool vm_tests(void)
{
    bool success = vmRun(initTests, runTests, printMessage, NULL);
    if (!success) {
        fprintf(stderr, "vmRun failed!\n");
        return false;
    }
    if (!initCalled) {
        fprintf(stderr, "VM didn't call init function!\n");
        return false;
    }
    if (!runCalled) {
        fprintf(stderr, "VM didn't call run function!\n");
        return false;
    }
    if (messagePrinted) {
        fprintf(stderr, "Lua has printed some unexpected messages!\n");
        return false;
    }
    return true;
}
