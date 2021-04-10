// sourceloctable.h: source location table definition
// license information in LICENSE
#ifndef __PVM_SOURCELOCTABLE_H__
#define __PVM_SOURCELOCTABLE_H__
#define SLTABLE_INIT_SIZE 10
#include <inttypes.h>

typedef struct _SourceLocTable SourceLocTable;
typedef struct _SourceLocTableEntry SourceLocTableEntry;

struct _SourceLocTable
{
    SourceLocTableEntry** entries;
    uint32_t length;
    uint32_t capacity;
};

struct _SourceLocTableEntry
{
    uint32_t line;
    uint32_t index;
};

SourceLocTable* source_loc_table_new ();
void source_loc_table_add (SourceLocTable* sltable, uint32_t index,
                           uint32_t line);
void source_loc_table_free (SourceLocTable* sltable);

#endif /* __PVM_SOURCELOCTABLE_H__ */