#include <htd/primitives/primitives.h>
#include <htd/data_structure/dynamic_array.h>

#include <stdlib.h>
#include <memory.h>

const usize GROWTH_FACTOR = 2;
const usize START_LEN = 32;

void dynarr_init(DynamicArray* arr, usize data_size) {
    arr->data_size = data_size;
    arr->len = 0;
    arr->capacity = START_LEN;
    arr->data = (void*)malloc(arr->capacity * arr->data_size); 
}

void* dynarr_at(DynamicArray* arr, usize idx) {
    return &((u8*)arr->data)[arr->data_size * idx];
}

void dynarr_push(DynamicArray* arr, void* data) {
    if (arr->len >= arr->capacity) {
        arr->capacity *= GROWTH_FACTOR;
        arr->data = (void*)realloc(arr->data, arr->capacity * arr->data_size);
    }
    
    memcpy(&((u8*)arr->data)[arr->data_size * arr->len], data, arr->data_size);
    arr->len++;
}

void dynarr_free(DynamicArray* arr) {
    free(arr->data);
    arr->data = NULL;
    arr->capacity = 0;
    arr->len = 0;
}
