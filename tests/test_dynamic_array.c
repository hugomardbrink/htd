#include <htd/primitives/primitives.h>
#include <htd/data_structure/dynamic_array.h>

#include <assert.h>

int main() {
    DynamicArray i32_array;
    dynarr_init(&i32_array, sizeof(i32));
    
    for (i32 i = 0; i < 100; i++) {
        dynarr_push(&i32_array, &i);
    }

    assert(i32_array.len == 100);
    for (i32 i = 0; i < 100; i++) {
        assert(*(i32*)dynarr_at(&i32_array, i) == i);
    }

    for (i32 i = 0; i < 100; i++) {
        dynarr_pop(&i32_array);
        assert(i32_array.len == 99 - i);
    }

    for (i32 i = 0; i < 100; i++) {
        dynarr_push(&i32_array, &i);
    }
    dynarr_remove(&i32_array, 50);
    assert(i32_array.len == 99);
    assert(*(i32*)dynarr_at(&i32_array, 50) == 51);

    dynarr_free(&i32_array);
}
