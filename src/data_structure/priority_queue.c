#include <htd/data_structure/priority_queue.h>
#include <htd/primitives/primitives.h>
#include <stdlib.h>
#include <memory.h>

static const usize GROWTH_FACTOR = 2;
static const usize START_LEN = 32;

void prioq_init(PriorityQueue* prioq, usize data_size, isize (*compare)(void*, void*)) {
    prioq->capacity = START_LEN; 
    prioq->data_size = data_size;
    prioq->compare = compare;
    prioq->len = 0;
    prioq->data = malloc(data_size * prioq->capacity); 
}

static inline usize parent(usize i) {
    return (i - 1) / 2;
}

static inline usize left_child(usize i) {
    return 2 * i + 1;
}

static inline usize right_child(usize i) {
    return 2 * i + 2;
}

static void* get_element(PriorityQueue* prioq, usize idx) {
    return (u8*)prioq->data + idx * prioq->data_size;
}

static void swap_elements(PriorityQueue* prioq, usize i, usize j) {
    u8 temp[prioq->data_size];
    void* elem_i = get_element(prioq, i);
    void* elem_j = get_element(prioq, j);
    
    memcpy(temp, elem_i, prioq->data_size);
    memcpy(elem_i, elem_j, prioq->data_size);
    memcpy(elem_j, temp, prioq->data_size);
}

static void sift_up(PriorityQueue* prioq, usize idx) {
    if (idx == 0) return;
    
    usize p = parent(idx);
    void* current = get_element(prioq, idx);
    void* parent_elem = get_element(prioq, p);
    
    if (prioq->compare(current, parent_elem) < 0) {
        swap_elements(prioq, idx, p);
        sift_up(prioq, p);
    }
}

static void sift_down(PriorityQueue* prioq, usize idx) {
    usize smallest = idx;
    usize left = left_child(idx);
    usize right = right_child(idx);
    
    if (left < prioq->len && 
        prioq->compare(get_element(prioq, left), get_element(prioq, smallest)) < 0) {
        smallest = left;
    }
    
    if (right < prioq->len && 
        prioq->compare(get_element(prioq, right), get_element(prioq, smallest)) < 0) {
        smallest = right;
    }
    
    if (smallest != idx) {
        swap_elements(prioq, idx, smallest);
        sift_down(prioq, smallest);
    }
}

void prioq_push(PriorityQueue* prioq, void* data) {
    if (prioq->len >= prioq->capacity) {
        prioq->capacity *= GROWTH_FACTOR;
        prioq->data = realloc(prioq->data, prioq->data_size * prioq->capacity);
    }
    
    void* new_position = get_element(prioq, prioq->len);
    memcpy(new_position, data, prioq->data_size);
    prioq->len++;
    
    sift_up(prioq, prioq->len - 1);
}

void prioq_pop(PriorityQueue* prioq, void* val) {
    memcpy(val, prioq->data, prioq->data_size);
    
    prioq->len--;
    if (prioq->len > 0) {
        memcpy(prioq->data, get_element(prioq, prioq->len), prioq->data_size);
        sift_down(prioq, 0);
    }
}

void prioq_free(PriorityQueue* prioq) {
    free(prioq->data);
    prioq->data = NULL;
    prioq->capacity = 0;
    prioq->len = 0;
    prioq->data_size = 0;
    prioq->compare = NULL;
}

