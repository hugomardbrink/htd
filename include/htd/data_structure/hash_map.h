#ifndef HTD_HASH_MAP_H
#define HTD_HASH_MAP_H

#include <htd/primitives/primitives.h>

typedef struct {
    void* key;
    void* val;
} HashMapEntry;

typedef struct {
    HashMapEntry* table;

    usize len;
    usize capacity;
    usize key_size;
    usize val_size;
} HashMap;

void hmap_init(HashMap* hmap, usize key_size, usize val_size);

void hmap_put(HashMap* hmap, const void* key, const void* val);

void* hmap_get(HashMap* hmap, const void* key);

void hmap_free(HashMap* hmap);

#endif // HTD_HASH_MAP_H
