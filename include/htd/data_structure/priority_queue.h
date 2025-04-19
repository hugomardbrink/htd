#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <htd/primitives/primitives.h>

typedef struct {
    void* data;

    isize (*compare)(void*, void*);
    usize data_size; 
    usize capacity;
    usize len;
} PriorityQueue;

void prioq_init(PriorityQueue* prioq, usize data_size, isize (*compare)(void*, void*));

void prioq_push(PriorityQueue* prioq, void* data);

void prioq_pop(PriorityQueue* prioq, void* val);

void prioq_free(PriorityQueue* prioq);

#endif // PRIORITY_QUEUE_H
