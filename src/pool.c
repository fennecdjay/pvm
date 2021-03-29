// pool.c: constant pool implementation as defined by pool.h
// license information in LICENSE
#include "pool.h"
#include <stdlib.h>
#include <uchar.h>
#include "utils.h"

static PoolEntry* pool_entry_new (EntryType type);

Pool* pool_new ()
{
    Pool* pool    = malloc (sizeof (Pool));
    pool->entries = malloc (sizeof (PoolEntry*) * POOL_INIT_ENTRIES_SIZE);
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
        pool->entries_count++;
    }

    pool->entries[pool->entries_count] = entry;
}

void pool_free (Pool* pool)
{
    return_if_null (pool);
    for (uint64_t i = 0; i < pool->entries_count; i++)
    {
        pool_entry_free (pool->entries[i]);
    }

    free (pool->entries);
    free (pool);
}

static PoolEntry* pool_entry_new (EntryType type)
{
    PoolEntry* entry = malloc (sizeof (PoolEntry));
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

PoolEntry* pool_entry_new_utf8 (char* str, uint64_t len)
{
    PoolEntry* entry = pool_entry_new (PVM_POOL_ENTRY_TYPE_UTF8);
    entry->value.str = str;
    entry->len       = len;
    return entry;
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
            asprintf (&end, "value=%s]", entry->value.str);
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