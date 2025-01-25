#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef __BORLANDC__
#pragma option -w-ccc
#pragma option -w-sig
#pragma option -w-rch
#endif

#ifdef _MSC_VER
#pragma warning(disable:4127)
#pragma warning(disable:4255)
#pragma warning(disable:4777)
#endif

#ifdef __WATCOMC__
#pragma disable_message(201)
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wlong-long"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wc99-extensions"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wconversion"
#endif

int pstdint_main(void);

#define __TEST_PSTDINT_FOR_CORRECTNESS
#define main pstdint_main
#include "pstdint.h"
#undef main

int main()
{
    int success = 1;
    char buf[256];
    int r;

    remove("t_base.run");

    /* pstdint.h */

    r = pstdint_main();
    #define CHECKEQSIZE(type1, type2) \
        if (sizeof(type1) != sizeof(type2)) { \
            r = EXIT_FAILURE; \
            fprintf(stderr, "sizeof " #type1 " (%d) != sizeof " #type2 " (%d)!\n", \
                (int)sizeof(type1), (int)sizeof(type2)); \
        }
    CHECKEQSIZE(intptr_t, void*)
    CHECKEQSIZE(uintptr_t, void*)
    #define CHECKGESIZE(type1, type2) \
        if (sizeof(type1) < sizeof(type2)) { \
            r = EXIT_FAILURE; \
            fprintf(stderr, "sizeof " #type1 " (%d) < sizeof " #type2 " (%d)!\n", \
                (int)sizeof(type1), (int)sizeof(type2)); \
        }
    CHECKGESIZE(intmax_t, char)
    CHECKGESIZE(intmax_t, short)
    CHECKGESIZE(intmax_t, int)
    CHECKGESIZE(intmax_t, long)
    CHECKGESIZE(intmax_t, size_t)
    CHECKGESIZE(intmax_t, ptrdiff_t)
    CHECKGESIZE(intmax_t, void*)
    CHECKGESIZE(uintmax_t, unsigned char)
    CHECKGESIZE(uintmax_t, unsigned short)
    CHECKGESIZE(uintmax_t, unsigned int)
    CHECKGESIZE(uintmax_t, unsigned long)
    CHECKGESIZE(uintmax_t, size_t)
    CHECKGESIZE(uintmax_t, ptrdiff_t)
    CHECKGESIZE(uintmax_t, void*)
    #define CHECKBITS(type, bits) \
        if (sizeof(type) * CHAR_BIT != bits) { \
            r = EXIT_FAILURE; \
            fprintf(stderr, "sizeof " #type " (%d) is not " #bits " bits (CHAR_BIT = %d)!\n", \
                (int)sizeof(type), (int)(CHAR_BIT)); \
        }
    CHECKBITS(int8_t, 8)
    CHECKBITS(uint8_t, 8)
    CHECKBITS(int16_t, 16)
    CHECKBITS(uint16_t, 16)
    CHECKBITS(int32_t, 32)
    CHECKBITS(uint32_t, 32)
  #ifdef stdint_int64_defined
    CHECKBITS(int64_t, 64)
    CHECKBITS(uint64_t, 64)
  #endif
    #define CHECKVALUE(format, value, expect) \
        sprintf(buf, "%" format, value); \
        if (strcmp(buf, expect) != 0) { \
            r = EXIT_FAILURE; \
            fprintf(stderr, "value of " #value " is not \"" expect "\" but \"%s\"!\n", buf); \
        }
    CHECKVALUE("d", INT8_MIN, "-128")
    CHECKVALUE("d", INT8_MAX, "127")
    CHECKVALUE("u", UINT8_MAX, "255")
    CHECKVALUE(PRINTF_INT16_MODIFIER "d", INT16_MIN, "-32768")
    CHECKVALUE(PRINTF_INT16_MODIFIER "d", INT16_MAX, "32767")
    CHECKVALUE(PRINTF_INT16_MODIFIER "u", UINT16_MAX, "65535")
    CHECKVALUE(PRINTF_INT32_MODIFIER "d", INT32_MIN, "-2147483648")
    CHECKVALUE(PRINTF_INT32_MODIFIER "d", INT32_MAX, "2147483647")
    CHECKVALUE(PRINTF_INT32_MODIFIER "u", UINT32_MAX, "4294967295")
  #ifdef stdint_int64_defined
    CHECKVALUE(PRINTF_INT64_MODIFIER "d", INT64_MIN, "-9223372036854775808")
    CHECKVALUE(PRINTF_INT64_MODIFIER "d", INT64_MAX, "9223372036854775807")
    CHECKVALUE(PRINTF_INT64_MODIFIER "u", UINT64_MAX, "18446744073709551615")
  #endif
    if (r != EXIT_SUCCESS)
        success = 0;
    else
        printf("pstdint.h tests passed!\n");

    /* done */

    if (success) {
        FILE* f = fopen("t_base.run", "w");
        if (!f) {
            fprintf(stderr, "can't write \"%s\": %s\n", "t_base.run", strerror(errno));
            return EXIT_FAILURE;
        }
        fwrite("1", 1, 1, f);
        fclose(f);
    }

    return r;
}
