// sourceloctable.c: source location table implementation
// as defined by sourceloctable.h
// license information in LICENSE
#include "sourceloctable.h"
#include <stdlib.h>
#include "utils.h"

SourceLocTable* source_loc_table_new ()
{
    SourceLocTable* slt = checked_malloc (sizeof (SourceLocTable));
    slt->length         = 0;
    slt->keys           = checked_malloc (sizeof (uint64_t) * SLTABLE_INIT_SIZE);
    slt->values         = checked_malloc (sizeof (uint64_t) * SLTABLE_INIT_SIZE);
    slt->capacity       = SLTABLE_INIT_SIZE;
    return slt;
}

void source_loc_table_add (SourceLocTable* sltable, uint64_t index,
                           uint64_t line)
{
    if (sltable->length == sltable->capacity)
    {
        sltable->keys   = realloc (sltable->keys, sltable->capacity *= 2);
        sltable->values = realloc (sltable->values, sltable->capacity);
        sltable->length++;
    }

    sltable->keys[sltable->length - 1]   = index;
    sltable->values[sltable->length - 1] = line;
}

void source_loc_table_free (SourceLocTable* sltable)
{
    return_if_null (sltable);
    free (sltable->keys);
    free (sltable->values);
    free (sltable);
}