#ifndef SCL_STRING_H
#define SCL_STRING_H

#include "types.h"
#include "array.h"

typedef struct scl_string_t {
    scl_array_t *source;
} scl_string_t;

#endif