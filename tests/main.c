#include <drunkfly/common.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define RUN_FILE_NAME "t_base.run"

bool pstdint_tests(void);
bool vm_tests(void);

int main(void)
{
    bool success = true;

    remove(RUN_FILE_NAME);

    if (!pstdint_tests())
        success = false;
    if (!vm_tests())
        success = false;

    if (success) {
        FILE* f = fopen(RUN_FILE_NAME, "w");
        if (!f) {
            fprintf(stderr, "can't write \"%s\": %s\n", RUN_FILE_NAME, strerror(errno));
            return EXIT_FAILURE;
        }
        fwrite("1", 1, 1, f);
        fclose(f);
        printf("=== VM TESTS SUCCEEDED ===\n");
    }

    return (success ? EXIT_SUCCESS : EXIT_FAILURE);
}
