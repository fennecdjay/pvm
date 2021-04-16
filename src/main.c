// main.c: entry point to pvm
// written by apachejuice
// license information in LICENSE
#include "pbc/byteUtils.h"
#include "utils/utils.h"
#include <stdio.h>

int main (int argc, char *argv[])
{
    printf ("Based.\n");
    int8_t bytes[] = {
        0xBA,
        0x5E,
        0xDD,
        0xAF,
    };

    pvm_panicf ("foobar: %d", 43);
    printf ("%u\n", byte_utils_to_u32 (bytes));
}
