#include <htd/primitives/primitives.h>
#include <htd/data_structure/hash_map.h>

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

const f32 LOAD_FACTOR = 0.7f;
const f32 TOMBSTONE_FACTOR = 0.2f;
const usize PRIME_CAPACITIES[] = {
    17, 37, 67, 131, 257, 521, 1031, 2053, 4099, 8209,
    16411, 32771, 65537, 131101, 262147, 524309, 1048583, 
    2097169, 4194301, 8388617, 16777259, 33554467, 67108879,
};

typedef struct {
    void* key;
    void* val;
    bool is_deleted;
} HashMapEntry;

usize hash1(const void* key, usize len) {
    const u8 *data = (const u8*)key;
    u32 h = 0x811c9dc5; 
    const u32 c1 = 0xcc9e2d51;
    const u32 c2 = 0x1b873593;

    const usize nblocks = len / 4;
    for (usize i = 0; i < nblocks; i++) {
        u32 k;
        memcpy(&k, data, sizeof(u32));
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
        for (usize i = 0; i < tail_size; i++) {
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

usize hash2(HashMap* hmap, const void* key, usize len) {
    usize h1 = hash1(key, len);

    return 1 + (h1 % (hmap->capacity - 1));
}

void hmap_init(HashMap* hmap, usize key_size, usize val_size) {
    hmap->len = 0;
    hmap->capacity = PRIME_CAPACITIES[0];
    hmap->prime_idx = 0;
    hmap->key_size = key_size;
    hmap->val_size = val_size;
    hmap->tombstone_count = 0;

    hmap->table = (HashMapEntry*) calloc(hmap->capacity, sizeof(HashMapEntry)); 
}

void rehash(HashMap* hmap, usize old_capacity) {
    HashMapEntry* table = hmap->table;
    if (hmap->len == 0) {
         for (usize i = 0; i < old_capacity; i++) {
              if (table[i].key != NULL) { 
                   free(table[i].key);
                   table[i].key = NULL;
                   table[i].val = NULL;
                   table[i].is_deleted = false;
              }
         }
         hmap->tombstone_count = 0;
         return;
    }

    HashMapEntry* old_table = hmap->table;
    hmap->table =  (HashMapEntry*) calloc(hmap->capacity, sizeof(HashMapEntry)); 
    hmap->len = 0;
    hmap->tombstone_count = 0;
    // TODO: capacity is too big for old table when increasing
    for (usize i = 0; i < old_capacity; i++) {
        if(old_table[i].key != NULL) {
            if (old_table[i].is_deleted) {
                free(old_table[i].key);
            } else { 
                hmap_put(hmap, old_table[i].key, old_table[i].val);
                free(old_table[i].key);
                free(old_table[i].val);
            }
        }
    }

    free(old_table);
}

void hmap_put(HashMap* hmap, const void* key, const void* val) {
    if (hmap->len >= (hmap->capacity * LOAD_FACTOR)) {
        hmap->prime_idx++;
        hmap->capacity = PRIME_CAPACITIES[hmap->prime_idx];
        rehash(hmap, PRIME_CAPACITIES[hmap->prime_idx - 1]);
    } else if (hmap->tombstone_count >= (hmap->capacity * TOMBSTONE_FACTOR)) {
        rehash(hmap, hmap->capacity);
    }

    usize h1 = hash1(key, hmap->key_size) % hmap->capacity;
    usize h2 = hash2(hmap, key, hmap->key_size);

    HashMapEntry* table = hmap->table;
    isize fst_tombstone = -1;

    for (usize i = 0; i < hmap->capacity; i++) {
        usize idx = (h1 + i * h2) % hmap->capacity;
        
        if (table[idx].is_deleted && fst_tombstone == -1) {
            fst_tombstone = idx;
        }

        if (table[idx].key != NULL && !table[idx].is_deleted && 
            memcmp(table[idx].key, key, hmap->key_size) == 0) {

            free(table[idx].val);
            table[idx].val = malloc(hmap->val_size);
            memcpy(table[idx].val, val, hmap->val_size);
            return;
        }


        if (table[idx].key == NULL) {
            if (fst_tombstone != -1) {
                idx = fst_tombstone;
                hmap->tombstone_count--;
            } else {
                table[idx].key = malloc(hmap->key_size);
            } 
            
            table[idx].val = malloc(hmap->val_size);
            memcpy(table[idx].key, key, hmap->key_size);
            memcpy(table[idx].val, val, hmap->val_size);
            table[idx].is_deleted = false;
            hmap->len++;
            return;
        }

    }

    assert(false && "Error: HashMap is full, memory allocation probably failed"); 
}


void hmap_remove(HashMap* hmap, const void* key) {
    if (hmap->tombstone_count >= (hmap->capacity * TOMBSTONE_FACTOR)) {
        rehash(hmap, hmap->capacity);
    }

    usize h1 = hash1(key, hmap->key_size) % hmap->capacity;
    usize h2 = hash2(hmap, key, hmap->key_size);
    HashMapEntry* table = hmap->table;

    for (usize i = 0; i < hmap->capacity; i++) {
        usize idx = (h1 + i * h2) % hmap->capacity;
        
        if (table[idx].key == NULL && !table[idx].is_deleted) {
            return;
        }
        
        if (table[idx].key != NULL && !table[idx].is_deleted &&
            memcmp(table[idx].key, key, hmap->key_size) == 0) {
            table[idx].is_deleted = true;
            hmap->tombstone_count++;

            free(table[idx].val);
            table[idx].val = NULL;
            hmap->len--;
            return;
        }
    }
}

void* hmap_get(HashMap* hmap, const void* key) {
    usize h1 = hash1(key, hmap->key_size) % hmap->capacity;
    usize h2 = hash2(hmap, key, hmap->key_size);
    HashMapEntry* table = hmap->table;
  
    for (usize i = 0; i < hmap->capacity; i++) {
        usize idx = (h1 + i * h2) % hmap->capacity;
        
        if (table[idx].key == NULL && !table[idx].is_deleted) {
            return NULL;
        }
        
        if (table[idx].key != NULL && !table[idx].is_deleted && 
            memcmp(table[idx].key, key, hmap->key_size) == 0) {
            return table[idx].val;
        }
    }
    
    return NULL;
}

bool hmap_contains(HashMap* hmap, const void* key) {
    usize h1 = hash1(key, hmap->key_size) % hmap->capacity;
    usize h2 = hash2(hmap, key, hmap->key_size);
    HashMapEntry* table = hmap->table;
    
    for (usize i = 0; i < hmap->capacity; i++) {
        usize idx = (h1 + i * h2) % hmap->capacity;
        if (table[idx].key == NULL && !table[idx].is_deleted) {
            return false;
        } else if (table[idx].key != NULL && !table[idx].is_deleted && memcmp(table[idx].key, key, hmap->key_size) == 0) {
            return true; 
        }
    }
    
    return false;
}

void hmap_free(HashMap* hmap) {
    HashMapEntry* table = hmap->table;

    for (usize i = 0; i < hmap->capacity; i++) {
        if (table[i].key != NULL) {
            free(table[i].key);
            free(table[i].val);
        }
    }
    free(hmap->table);
    hmap->table = NULL;
    hmap->capacity = 0;
    hmap->key_size = 0;
    hmap->val_size = 0;
    hmap->prime_idx = 0;
    hmap->len = 0;
}
