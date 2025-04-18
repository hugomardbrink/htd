#include <htd/primitives/primitives.h>
#include <htd/data_structure/hash_map.h>

#include <assert.h>

int main() {
    HashMap i32_i64_hmap;
    hmap_init(&i32_i64_hmap, sizeof(i32), sizeof(i64));

    for (i32 i = 0; i < 100; i++) {
        i64 v = (i64)i * 2;
        hmap_put(&i32_i64_hmap, &i, &v);
    }

    for (i32 i = 0; i < 100; i++) {
        i64 expected_value = (i64)i * 2;
        i64* retrieved_value = (i64*)hmap_get(&i32_i64_hmap, &i);
        
        assert(retrieved_value != NULL && *retrieved_value == expected_value);
    }

    for (i32 i = 0; i < 100; i+=2) {
        hmap_remove(&i32_i64_hmap, &i);
        i64* retrieved_value = (i64*)hmap_get(&i32_i64_hmap, &i);
        assert(retrieved_value == NULL);
    }
    for (i32 i = 1; i < 100; i+=2) {
        i64 expected_value = (i64)i * 2;
        i64* retrieved_value = (i64*)hmap_get(&i32_i64_hmap, &i);
        
        assert(retrieved_value != NULL && *retrieved_value == expected_value);
    }

    hmap_free(&i32_i64_hmap);

    return 0;
}
