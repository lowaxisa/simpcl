#include <stdio.h>
#include "../../include/simpcl/simpcl.h"

uint16_t counter = 0;

void routine() {
    for (int i = 0; i < 4; i++) {
        printf("\n");
        scl_coroutine_t *c = &scl_coroutines[scl_current_coroutine_pid];
        for (uint16_t j = 0; j < MAX_MESSAGE; j++) {
            if (c->msg[j].occupied) {
                printf("I am %d and i received signal %d, from %d\n", scl_current_coroutine_pid, c->msg[j].signal, c->msg[j].pid);
                c->msg[j].occupied = false;
            }
        }
        printf("I am %d, current counter: %d\nNow i sleep for 200ms...\n", scl_current_coroutine_pid, counter);
        counter++;
        for (int j = 0; j < 4; j++) {
            printf("i send %d for %d\n", counter, j);
            scl_coroutine_send(j, 0, NULL);
        }
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