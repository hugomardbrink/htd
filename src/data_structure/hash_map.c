#include <htd/primitives/primitives.h>
#include <htd/data_structure/hash_map.h>

#include <stdlib.h>
#include <memory.h>

const usize START_LEN = 16;
const usize GROWTH_FACTOR = 2;

usize hash(const u8* key, usize len) {
    const u8 *data = (const u8*)key;
    u32 h = 0x811c9dc5; 
    const u32 c1 = 0xcc9e2d51;
    const u32 c2 = 0x1b873593;

    const usize nblocks = len / 4;
    for (usize i = 0; i < nblocks; i++) {
        u32 k = *((u32*)data);
        data += 4;

        k *= c1;
        k = (k << 15) | (k >> 17);
        k *= c2;

        h ^= k;
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }

    const usize tail_size = len & 3;
    u32 k1 = 0;
    if (tail_size > 0) {
        for (usize i = 0; i < tail_size; ++i) {
            k1 ^= data[i] << (i * 8);
        }

        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= c2;
        h ^= k1;
    }

    h ^= len;
    h = h ^ (h >> 16);
    h *= 0x85ebca6b;
    h = h ^ (h >> 13);
    h *= 0xc2b2ae35;
    h = h ^ (h >> 16);

    return h;
}

void hmap_init(HashMap* hmap, usize key_size, usize val_size) {
    hmap->len = 0;
    hmap->capacity = START_LEN;
    hmap->key_size = key_size;
    hmap->val_size = val_size;

    hmap->table = (HashMapEntry*)malloc(sizeof(HashMapEntry) * hmap->capacity);
    for (usize i = 0; i < hmap->capacity; ++i) {
        hmap->table[i].key = NULL;
        hmap->table[i].val = NULL;
    }
}

void resize(HashMap* hmap) {
    usize old_capacity = hmap->capacity;
    HashMapEntry* old_table = (HashMapEntry*)malloc(sizeof(HashMapEntry) * old_capacity);
    memcpy(old_table, hmap->table, sizeof(HashMapEntry) * old_capacity);
    
    hmap->capacity *= GROWTH_FACTOR;
    hmap->table = (HashMapEntry*)realloc(hmap->table, sizeof(HashMapEntry) * hmap->capacity);
    
    for (usize i = 0; i < hmap->capacity; i++) {
        hmap->table[i].key = NULL;
        hmap->table[i].val = NULL;
    }
    
    // Reset length, re-adding
    hmap->len = 0;
    for (usize i = 0; i < old_capacity; i++) {
        if (old_table[i].key != NULL) {
            hmap_put(hmap, old_table[i].key, old_table[i].val);
            
            free(old_table[i].key);
            free(old_table[i].val);
        }
    }
    
    free(old_table);
}

void hmap_put(HashMap* hmap, const void* key, const void* val) {
    if (hmap->len >= hmap->capacity) {
        resize(hmap);
    }

    usize idx = hash(key, hmap->key_size) % hmap->capacity;
    
    while (hmap->table[idx].key != NULL) {
        if (memcmp(hmap->table[idx].key, key, hmap->key_size) == 0) {
            free(hmap->table[idx].val);
            hmap->table[idx].val = malloc(hmap->val_size);
            memcpy(hmap->table[idx].val, val, hmap->val_size);
            return;
        }
        idx = (idx + 1) % hmap->capacity;
    }
    
    hmap->table[idx].key = malloc(hmap->key_size);
    memcpy(hmap->table[idx].key, key, hmap->key_size);
    hmap->table[idx].val = malloc(hmap->val_size);
    memcpy(hmap->table[idx].val, val, hmap->val_size);

    hmap->len++;
}

void* hmap_get(HashMap* hmap, const void* key) {
    usize idx = hash(key, hmap->key_size) % hmap->capacity;
   
    while (hmap->table[idx].key != NULL) {
        if (memcmp(hmap->table[idx].key, key, hmap->key_size) == 0) {
            return hmap->table[idx].val;
        }
        idx = (idx + 1) % hmap->capacity;
    }
    
    return NULL;
}

void hmap_free(HashMap* hmap) {
    for (usize i = 0; i < hmap->capacity; ++i) {
        if (hmap->table[i].key != NULL) {
            free(hmap->table[i].key);
            free(hmap->table[i].val);
        }
    }
    free(hmap->table);
    hmap->table = NULL;
    hmap->capacity = 0;
    hmap->len = 0;
}
