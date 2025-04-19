#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <htd/primitives/primitives.h>

typedef struct {
    void *data;
     
    usize len;
    usize data_size;
    usize capacity;
} DynamicArray;

void dynarr_init(DynamicArray* arr, usize size);

void* dynarr_at(DynamicArray* arr, usize idx);

void dynarr_set(DynamicArray* arr, usize idx, void* data);

void dynarr_copy(DynamicArray* src, DynamicArray* dst);

void dynarr_push(DynamicArray* arr, void* data);

void dynarr_free(DynamicArray* arr);

void* dynarr_last(DynamicArray* arr);

void dynarr_remove(DynamicArray* arr, usize idx);

void* dynarr_pop(DynamicArray* arr);

#endif // DYNAMIC_ARRAY_H
