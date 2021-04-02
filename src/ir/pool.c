// pool.c: constant pool implementation as defined by pool.h
// license information in LICENSE
#include "pool.h"
#include <stdlib.h>
#include <stdio.h>
#include <uchar.h>
#include "utils/utils.h"

static PoolEntry* pool_entry_new (EntryType type);

Pool* pool_new ()
{
    Pool* pool = checked_malloc (sizeof (Pool));
    pool->entries =
        checked_malloc (sizeof (PoolEntry*) * POOL_INIT_ENTRIES_SIZE);
    pool->entries_capacity = POOL_INIT_ENTRIES_SIZE;
    pool->entries_count    = 0;
    return pool;
}

void pool_add_entry (Pool* pool, PoolEntry* entry)
{
    if (pool->entries_count == pool->entries_capacity)
    {
        pool->entries = realloc (
            pool->entries, sizeof (PoolEntry*) * (pool->entries_capacity * 2));
        pool->entries_capacity = pool->entries_capacity * 2;
    }

    pool->entries[pool->entries_count++] = entry;
}

void pool_free (Pool* pool)
{
    return_if_null (pool);
    for (uint32_t i = 0; i < pool->entries_count; i++)
    {
        pool_entry_free (pool->entries[i]);
    }

    free (pool->entries);
    free (pool);
}

static PoolEntry* pool_entry_new (EntryType type)
{
    PoolEntry* entry = checked_malloc (sizeof (PoolEntry));
    entry->type      = type;
    return entry;
}

PoolEntry* pool_entry_new_long (uint64_t l)
{
    PoolEntry* entry = pool_entry_new (PVM_POOL_ENTRY_TYPE_LONG);
    entry->value.l   = l;
    entry->len       = sizeof (l);
    return entry;
}

PoolEntry* pool_entry_new_utf32 (char* str, uint32_t len)
{
    PoolEntry* entry = pool_entry_new (PVM_POOL_ENTRY_TYPE_UTF8);
    entry->value.str = str;
    entry->len       = len;
    return entry;
}

bool pool_has_entry (Pool* pool, uint32_t idx)
{
    return pool->entries_count > idx;
}

const char* pool_entry_to_string (PoolEntry* entry)
{
    char* begin;
    asprintf (&begin, "Entry[type=%d ", entry->type);
    char* end;
    switch (entry->type)
    {
        case PVM_POOL_ENTRY_TYPE_UTF8:
        {
            asprintf (&end, "value='%s']", entry->value.str);
            break;
        }

        case PVM_POOL_ENTRY_TYPE_LONG:
        {
            asprintf (&end, "value=%lX]", entry->value.l);
            break;
        }
    }

    char* result;
    asprintf (&result, "%s%s", begin, end);
    return result;
}

void pool_entry_free (PoolEntry* entry)
{
    switch (entry->type)
    {
        case PVM_POOL_ENTRY_TYPE_UTF8:
        {
            free (entry->value.str);
        }
    }

    free (entry);
}