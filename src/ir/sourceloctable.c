// sourceloctable.c: source location table implementation
// as defined by sourceloctable.h
// license information in LICENSE
#include "sourceloctable.h"
#include <stdlib.h>
#include "utils/utils.h"

static SourceLocTableEntry* source_loc_table_entry_new (uint32_t line,
                                                        uint32_t index)
{
    SourceLocTableEntry* e = checked_malloc (sizeof (SourceLocTableEntry));
    e->line                = line;
    e->index               = index;
    return e;
}

SourceLocTable* source_loc_table_new ()
{
    SourceLocTable* slt = checked_malloc (sizeof (SourceLocTable));
    slt->length         = 0;
    slt->entries =
        checked_calloc (SLTABLE_INIT_SIZE, sizeof (SourceLocTableEntry*));
    slt->capacity = SLTABLE_INIT_SIZE;
    return slt;
}

void source_loc_table_add (SourceLocTable* sltable, uint32_t line,
                           uint32_t index)
{
    if (sltable->length == sltable->capacity)
    {
        sltable->entries =
            realloc (sltable->entries,
                     sizeof (SourceLocTableEntry*) * (sltable->capacity *= 2));
    }

    sltable->length++;
    sltable->entries[sltable->length - 1] =
        source_loc_table_entry_new (line, index);
}

void source_loc_table_free (SourceLocTable* sltable)
{
    return_if_null (sltable);
    free (sltable->entries);
    free (sltable);
}