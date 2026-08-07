#include <stdio.h>
#include "../include/simpcl/simpcl.h"

void cb(void *e) {
    printf("%X", *(int *) e);
}

bool_t sort_cb(void *a, void *b) {
    return *(int *) a > *(int *) b;
}

void populate_array(scl_array_t *a, uint64_t n) {
    for (int i = n - 1; i > 0; i--) {
        scl_array_push(a, &i);
        printf("%d ", i);
    }
}

void feach_cb(void *d) {
    printf("%d ", *(int *) d);
}

bool_t cmp_cb(void *a, void *b) {
    return *(int *) a == *(int *) b;
}

int main() {
    scl_array_t *a = scl_array_new(sizeof(int));

    int x = 1000;
    populate_array(a, 16);
    scl_array_swap(a, 14, 3, &x);
    printf("numbers in array\n");
    scl_array_print(a, cb);
    x = 5;
    printf("current x: %d\n", x);
    scl_array_pop(a, &x);
    printf("new x: %d\n", x);
    printf("numbers in array\n");
    scl_array_print(a, cb);
    scl_array_remove(a, 5, &x);
    scl_array_insert(a, 5, &x);
    printf("numbers in array\n");
    scl_array_print(a, cb);
    scl_array_reserve(a, 32);
    printf("array capacity: %d\n", a->capacity);
    scl_array_shrink(a);
    printf("array capacity: %d\n", a->capacity);
    scl_array_sort(a, sort_cb, &x);
    printf("numbers in array\n");
    scl_array_print(a, cb);
    scl_array_foreach(a, feach_cb);
    
    scl_array_t *copy = scl_array_copy(a, NULL);
    printf("numbers in array copy\n");
    scl_array_print(copy, cb);

    printf("\n\nArray A = array B? ");
    if (scl_array_compare(a, copy, cmp_cb)) printf("true\n");

    scl_array_destroy(a);
    scl_array_destroy(copy);

    return 0;
}