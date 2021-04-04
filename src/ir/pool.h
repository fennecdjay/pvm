// pool.h: constant pool definition
// license information in LICENSE
#ifndef __PVM_POOL_H__
#define __PVM_POOL_H__
#include <stdint.h>
#include <stdbool.h>
#define POOL_INIT_ENTRIES_SIZE 10
#include <inttypes.h>
#include <uchar.h>

typedef struct _Pool Pool;
typedef struct _PoolEntry PoolEntry;

typedef enum
{
    PVM_POOL_ENTRY_TYPE_UTF32,
    PVM_POOL_ENTRY_TYPE_LONG,
} EntryType;

struct _Pool
{
    PoolEntry** entries;
    uint32_t entries_count;
    uint32_t entries_capacity;
};

Pool* pool_new ();
void pool_add_entry (Pool* pool, PoolEntry* entry);
bool pool_has_entry (Pool* pool, uint32_t idx);
void pool_free (Pool* pool);

struct _PoolEntry
{
    EntryType type;
    uint32_t len;
    union
    {
        char* str;
        uint64_t l;
    } value;
};

PoolEntry* pool_entry_new_long (uint64_t l);
PoolEntry* pool_entry_new_utf32 (char* str, uint32_t len);
char* pool_entry_to_string (PoolEntry* entry);
void pool_entry_free (PoolEntry* entry);

#endif /* __PVM_POOL_H__ */