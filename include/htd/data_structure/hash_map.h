#ifndef HTD_HASH_MAP_H
#define HTD_HASH_MAP_H

#include <htd/primitives/primitives.h>

typedef struct {
    void* table;

    usize len;
    usize capacity;
    usize prime_idx;
    usize key_size;
    usize val_size;

    usize tombstone_count;
} HashMap;

void hmap_init(HashMap* hmap, usize key_size, usize val_size);

void hmap_put(HashMap* hmap, const void* key, const void* val);

void hmap_remove(HashMap* hmap, const void* key);

void* hmap_get(HashMap* hmap, const void* key);

bool hmap_contains(HashMap* hmap, const void* key);

void hmap_free(HashMap* hmap);

#endif // HTD_HASH_MAP_H
