#include <stdio.h>
#include "../include/simpcl/simpcl.h"

int main() {
    if (scl_coroutine_load("./m_module.so") == 0xFFFF) printf("Error\n");

    scl_coroutine_init();
    return 0;
}