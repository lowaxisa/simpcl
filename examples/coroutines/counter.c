#include <stdio.h>
#include "../../include/simpcl/simpcl.h"

uint16_t counter;

void routine() {
    for (int i = 0; i < 4; i++) {
        printf("I am %d, current counter: %d\nNow i sleep for 200ms...\n", scl_current_coroutine_pid, counter);
        counter++;
        scl_coroutine_sleep(200);
    }
}

int main() {
    scl_coroutine_summon(routine);
    scl_coroutine_summon(routine);
    scl_coroutine_summon(routine);
    scl_coroutine_summon(routine);

    scl_coroutine_init();

    return 0;
}