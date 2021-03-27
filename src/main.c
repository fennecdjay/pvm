// main.c: entry point to program
// license information in LICENSE
#include <stdio.h>
#include <string.h>
#include "scanner.h"

int main (int argc, char** argv)
{
    int8_t foo[] = {0xCA, 0x42, 0x0A, 0xBE, 0x44, 0xE8, 0x32, 0xB3};
    Scanner* s = scanner_new ("fpp", foo, 8);
    printf ("Foo: %u\n", scanner_look_u32 (s));
    scanner_free (s);
    return 0;
}