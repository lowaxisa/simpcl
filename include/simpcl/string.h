#ifndef SCL_STRING_H
#define SCL_STRING_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "types.h"
#include "array.h"

typedef struct scl_string_t {
    scl_array_t *source;
} scl_string_t;

// life-cycle
scl_string_t *scl_string_new();
void scl_string_destroy(scl_string_t *string);

// control
void scl_string_cappend(scl_string_t *string, const char *c_str);
void scl_string_append(scl_string_t *string, scl_string_t *source);

// memory
void scl_string_clear(scl_string_t *string);

// transformation
void scl_string_upper(scl_string_t *string);
void scl_string_lower(scl_string_t *string);

// misc
void scl_string_print(scl_string_t *string);
void scl_string_input(scl_string_t *string);

#endif