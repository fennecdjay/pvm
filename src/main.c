// main.c: entry point to program
// license information in LICENSE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pbcfile/parser.h"
#include "ir/code.h"
#include "interp/interpreter.h"
#include "utils/utils.h"
#include "encoding/utf8.h"

int main (int argc, char** argv)
{
    Parser* p = parser_new ("main.pbc");
    Code* c   = parser_parse (p);

    Function* main_func = NULL;
    for (uint32_t i = 0; i < c->functions_len; i++)
    {
        Function* f = c->functions[i];
        printf ("name: %s\n", c->pool->entries[f->name_ref]->value.str);
        if (strcmp (c->pool->entries[f->name_ref]->value.str, "Main") == 0)
        {
            main_func = f;
            break;
        }
    }

    pvm_assert (main_func != NULL, "No main function found");

    Interpreter* i = interp_new (c->pool);
    interp_run_function (i, main_func);

    interp_free (i);
    code_free (c);
    parser_free (p);
    return 0;
}