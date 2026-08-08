#include "../include/simpcl/string.h"

// life-cycle
scl_string_t *scl_string_new() {
    scl_string_t *string = malloc(sizeof(string));

    if (!string) return NULL;
    string->source = scl_array_new(sizeof(char));

    if (!string->source) {
        free(string);
        return NULL;
    }

    return string;
}

void scl_string_destroy(scl_string_t *string) {
    scl_array_destroy(string->source);
    free(string);
}



// control
void scl_string_cappend(scl_string_t *string, const char *c_str) {
    size_t c_str_length = strlen(c_str);

    for (size_t i = 0; i < c_str_length; i++) {
        scl_array_push(string->source, (void *) &c_str[i]);
    }
}

void scl_string_append(scl_string_t *string, scl_string_t *source) {
    scl_array_append(string->source, source->source, NULL);
}



// memory
void scl_string_clear(scl_string_t *string) {
    scl_array_clear(string->source);
}



// transformation
void scl_string_upper(scl_string_t *string) {
    size_t string_length = scl_array_length(string->source);

    for (size_t i = 0; i < string_length; i++) {
        char *c = (char *) scl_array_at(string->source, i);
        if (*c >= 'a' && *c <= 'z') {
            *c = *c - 32;
        }
    }
}

void scl_string_lower(scl_string_t *string) {
    size_t string_length = scl_array_length(string->source);

    for (size_t i = 0; i < string_length; i++) {
        char *c = (char *) scl_array_at(string->source, i);
        if (*c >= 'A' && *c <= 'Z') {
            *c = *c + 32;
        }
    }
}



// misc
void scl_string_print(scl_string_t *string) {
    write(1, scl_array_at(string->source, 0), scl_array_length(string->source));
}

void scl_string_input(scl_string_t *string) {
    char c;

    while ((c = getchar()) != EOF && c != '\n') {
        scl_array_push(string->source, (void *) &c);
    }
}