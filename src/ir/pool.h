// pool.h: constant pool definition
// license information in LICENSE
#ifndef __PVM_POOL_H__
#define __PVM_POOL_H__
#include <stdint.h>
#define POOL_INIT_ENTRIES_SIZE 10
#include <inttypes.h>
#include <uchar.h>

typedef struct _Pool Pool;
typedef struct _PoolEntry PoolEntry;

typedef enum
{
    PVM_POOL_ENTRY_TYPE_UTF8,
    PVM_POOL_ENTRY_TYPE_LONG,
} EntryType;

struct _Pool
{
    PoolEntry** entries;
    uint64_t entries_count;
    uint64_t entries_capacity;
};

Pool* pool_new ();
void pool_add_entry (Pool* pool, PoolEntry* entry);
void pool_free (Pool* pool);

struct _PoolEntry
{
    EntryType type;
    uint64_t len;
    union
    {
        char* str;
        uint64_t l;
    } value;
};

PoolEntry* pool_entry_new_long (uint64_t l);
PoolEntry* pool_entry_new_utf32 (char* str, uint64_t len);
const char* pool_entry_to_string (PoolEntry* entry);
void pool_entry_free (PoolEntry* entry);

#endif /* __PVM_POOL_H__ */