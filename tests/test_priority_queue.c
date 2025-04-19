#include <htd/primitives/primitives.h>
#include <htd/data_structure/priority_queue.h>

#include <assert.h>
#include <stdio.h>

isize i32_compare(void *a, void *b) {
    return *(int *)a - *(int *)b;
}

int main() {
    PriorityQueue pq;
    prioq_init(&pq, sizeof(i32), i32_compare);

    i32 values[] = {4, 1, 10, 33, 0};

    for (i32 i = 0; i < 5; i++) {
        prioq_push(&pq, &values[i]);
    }

    i32 correct[] = {0, 1, 4, 10, 33};
    for (i32 i = 0; i < 5; i++) {
        i32 value;
        prioq_pop(&pq, &value);
        printf("value: %d\n", value);
        assert(value == correct[i]);
    }

    prioq_free(&pq);
    return 0;
}
