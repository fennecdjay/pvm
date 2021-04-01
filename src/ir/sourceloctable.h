// sourceloctable.h: source location table definition
// license information in LICENSE
#ifndef __PVM_SOURCELOCTABLE_H__
#define __PVM_SOURCELOCTABLE_H__
#define SLTABLE_INIT_SIZE 10
#include <inttypes.h>

typedef struct _SourceLocTable SourceLocTable;

struct _SourceLocTable
{
    uint64_t* keys;
    uint64_t* values;
    uint64_t length;
    uint64_t capacity;
};

SourceLocTable* source_loc_table_new ();
void source_loc_table_add (SourceLocTable* sltable, uint64_t index,
                           uint64_t line);
void source_loc_table_free (SourceLocTable* sltable);

#endif /* __PVM_SOURCELOCTABLE_H__ */