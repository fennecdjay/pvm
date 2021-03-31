// main.c: entry point to program
// license information in LICENSE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pbcfile/parser.h"
#include "ir/code.h"
#include "encoding/utf8.h"

int main (int argc, char** argv)
{
    char* outstr;
    encode_utf8str (L"里里里里", 5, &outstr);
    printf ("%s\n", outstr);
    Parser* p = parser_new ("main.pbc");
    Code* c   = parser_parse (p);
    code_free (c);
    parser_free (p);
    return 0;
}