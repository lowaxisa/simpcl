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

    scl_string_clear(string);
    scl_string_t *target_string = scl_string_new();
    scl_string_cappend(target_string, "abc,");
    scl_string_cappend(string, "abc, abc, abc, abc,fwfwef, crver verv,ev,eveabc,ververv_dog_abc,\n");
    scl_array_t *array = scl_string_slice(string, target_string, 0, (size_t) -1);

    scl_string_clear(string);
    scl_string_cappend(string, "-aaa-");
    scl_string_t *t = scl_string_join(array, string);
    scl_string_print(t);
    scl_string_t *r = scl_string_replace(t, string, t);
    scl_string_print(r);

    scl_string_destroy(t);
    scl_string_destroy(r);
    
    for (size_t i = 0; i < scl_array_length(array); i++) {
        scl_string_t *s = *(scl_string_t **) scl_array_at(array, i);
        scl_string_print(s);
        scl_string_destroy(s);
    }

    scl_array_destroy(array);
    scl_string_destroy(string);
    scl_string_destroy(target_string);

    return 0;
}