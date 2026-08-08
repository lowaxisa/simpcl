#include <stdio.h>
#include "../include/simpcl/simpcl.h"

int main() {
    scl_string_t *string = scl_string_new();

    scl_string_cappend(string, "Hello, World!\nType a message: ");
    scl_string_print(string);

    scl_string_clear(string);
    scl_string_cappend(string, "Your message in lower/upper: ");
    scl_string_input(string);
    scl_string_cappend(string, "\n");
    scl_string_lower(string);
    scl_string_print(string);
    scl_string_upper(string);
    scl_string_print(string);

    scl_string_destroy(string);
    return 0;
}