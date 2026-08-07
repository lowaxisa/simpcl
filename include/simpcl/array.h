#ifndef SCL_ARRAY_H
#define SCL_ARRAY_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "types.h"

typedef struct scl_array_t {
    void *source;
    uint64_t size;
    uint64_t capacity;
    uint64_t type_size;
} scl_array_t;

// life-cycle
scl_array_t *scl_array_new(uint64_t type_size) {
    scl_array_t *a = (scl_array_t *) malloc(sizeof(scl_array_t));

    a->size = 0;
    a->capacity = 16;
    a->source = (void *) malloc(16 * type_size);
    a->type_size = type_size;

    return a;
}

void scl_array_destroy(scl_array_t *a) {
    free(a->source);
    free(a);
}

// memory
void scl_array_realloc(scl_array_t *a, uint64_t size) {
    a->capacity = size;
    a->source = realloc(a->source, size * a->type_size);
}

void scl_array_reserve(scl_array_t *a, uint64_t size) {
    if (size < a->capacity) return;
    scl_array_realloc(a, size);
}

void scl_array_shrink(scl_array_t *a) {
    scl_array_realloc(a, a->size);
}

void *scl_array_index(scl_array_t *a, uint64_t index) {
    return (char *) a->source + (a->type_size * index);
}

scl_array_t *scl_array_copy(scl_array_t *a, void (*callback)(void *a, void *b)) {
    scl_array_t *temp = scl_array_new(a->type_size);

    temp->size = a->size;
    scl_array_reserve(temp, a->capacity);

    memcpy(temp->source, a->source, a->type_size * a->size);

    if (!callback) return temp;
    for (uint64_t i = 0; i < a->size; i++) {
        callback(scl_array_index(temp, i), scl_array_index(a, i));
    }
    return temp;
}

// functions
void scl_array_push(scl_array_t *a, void *source) {
    if (a->size >= a->capacity) {
        scl_array_realloc(a, a->capacity * 2);
    }

    memcpy(scl_array_index(a, a->size), source, a->type_size);
    a->size++;
}

void scl_array_pop(scl_array_t *a, void *dest) {
    a->size--;
    memcpy(dest, scl_array_index(a, a->size), a->type_size);
}

void scl_array_swap(scl_array_t *a, uint64_t index, uint64_t target, void *buffer) {
    memcpy(buffer, scl_array_index(a, index), a->type_size);
    memcpy(scl_array_index(a, index), scl_array_index(a, target), a->type_size);
    memcpy(scl_array_index(a, target), buffer, a->type_size);
}

void scl_array_remove(scl_array_t *a, uint64_t index, void *dest) {
    memcpy(dest, scl_array_index(a, index), a->type_size);

    memmove(scl_array_index(a, index), scl_array_index(a, index + 1), (a->size - index) * a->type_size);
    a->size--;
}

void scl_array_insert(scl_array_t *a, uint64_t index, void *source) {
    scl_array_push(a, source);
    memmove(scl_array_index(a, index + 1), scl_array_index(a, index), (a->size - index) * a->type_size);
    memcpy(scl_array_index(a, index), source, a->type_size);
}

bool_t scl_array_compare(scl_array_t *a, scl_array_t *b, bool_t (*callback)(void *a, void *b)) {
    if (a->size != b->size) return false;

    for (uint64_t i = 0; i < a->size; i++) {
        if (!callback(scl_array_index(a, i), scl_array_index(b, i))) return false;
    }

    return true;
}

// sort
void scl_array_sort(scl_array_t *a, bool_t (*callback)(void *a, void *b), void *buffer) {
    while (true) {
        bool_t swaped = false;

        for (uint64_t j = 0; j + 1 < a->size; j++) {
            if (callback(scl_array_index(a, j), scl_array_index(a, j + 1))) {
                scl_array_swap(a, j, j + 1, buffer);
                swaped = true;
            }
        }

        if (!swaped) return;
    }
}

// misc
void scl_array_foreach(scl_array_t *a, void (*callback)(void *data)) {
    for (uint64_t i = 0; i < a->size; i++) {
        callback(scl_array_index(a, i));
    }
}

void scl_array_print(scl_array_t *a, void (*callback)(void *element)) { \
    uint64_t a_size = a->size;
    uint64_t a_half = (uint64_t) sqrt(a_size);
    a_half = (a_half > 8) ? 8 : a_half;

    for (uint64_t i = 0; i < a->size; i++) {
        callback(scl_array_index(a, i));

        if ((i + 1) % a_half == 0 && i != 0) {
            printf("\n");
            continue;
        }

        if (i + 1 < a->size) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

#endif