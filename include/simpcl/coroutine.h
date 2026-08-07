#ifndef SCL_COROUTINE_H
#define SCL_COROUTINE_H

#include <ucontext.h>
#include <setjmp.h>
#include <time.h>
#include <stdlib.h>
#include "types.h"
#include "misc.h"

#ifndef NULL
#define NULL (void *)0
#endif

#define STACK_SIZE 128 * 1024
#define MAX_COROUTINES 256

struct scl_coroutine_t;
typedef struct scl_coroutine_t {
    uint16_t pid;
    char status; // r = running, c = closed, s = sleeping
    uint64_t wake_at; // wake if time > wake_at (in ms)
    ucontext_t ctx;
    void *stack;
    void (*routine)();
} scl_coroutine_t;

// global vars
static jmp_buf scl_context_jmp;
static ucontext_t scl_context; // main context
static uint16_t scl_current_coroutine_pid = 0xFFFF;
static uint16_t scl_next_coroutine_pid = 0xFFFF;
static scl_coroutine_t scl_coroutines[MAX_COROUTINES];
static bool_t scl_coroutine_inited = false;

// helpers
scl_coroutine_t *scl_coroutine_find(uint16_t pid) {
    for (uint16_t i = 0; i < MAX_COROUTINES; i++) {
        if (scl_coroutines[i].pid == pid && scl_coroutines[i].stack != NULL) {
            return &scl_coroutines[i];
        }
    }
    return NULL;
}

uint16_t scl_coroutine_unused_pid() { // return coroutine if is alive
    for (uint16_t i = 0; i < MAX_COROUTINES; i++) {
        if (!scl_coroutines[i].stack) {
            return i;
        }
    }
}

// logic
void scl_coroutine_gc() {
    for (uint16_t i = 0; i < MAX_COROUTINES; i++) {
        scl_coroutine_t *c = &scl_coroutines[i];

        if (c->stack && c->status == 'c' && c->pid != scl_current_coroutine_pid) {
            free(c->stack);
            c->stack = NULL;
        }
    }
}

bool_t scl_coroutine_scheduler() {
    bool_t has_coroutines_alive = false;
    uint16_t curr_index = 0;
    for (uint16_t i = 0; i < MAX_COROUTINES; i++) {
        scl_coroutine_t *c = &scl_coroutines[i];
        if (!c) continue;

        if (c->status == 'r' || c->status == 's') {
            has_coroutines_alive = true;
        }

        if (c->pid == scl_current_coroutine_pid) curr_index = i;
    }

    if (!has_coroutines_alive) return false;

    while (true) {
        bool_t next_coroutine_find = false;
        uint64_t sleep_time = 0xFFFFFFFFFFFFFFFF;
        uint64_t current_time = scl_ms();

        for (uint16_t i = 0; i < MAX_COROUTINES; i++) {
            uint16_t index = (curr_index + i + 1) % MAX_COROUTINES;
            scl_coroutine_t *c = &scl_coroutines[index];
            if (!c->stack) continue;

            if (c->status == 's') {

                if (current_time >= c->wake_at) {
                    c->status = 'r';
                } else {
                    uint64_t diff = c->wake_at - current_time;
                    sleep_time = (sleep_time > diff) ? diff : sleep_time;
                }
            }

            if (c->status == 'r') {
                next_coroutine_find = true;
                scl_next_coroutine_pid = c->pid;
                break;
            }
        }

        if (next_coroutine_find) break;

        struct timespec time;
        time.tv_sec = sleep_time / 1000;
        time.tv_nsec = (sleep_time % 1000) * 1000000L;
        nanosleep(&time, NULL);
    }

    return true;
}

void scl_coroutine_yield() {
    scl_coroutine_gc();
    if (!scl_coroutine_scheduler()) longjmp(scl_context_jmp, 1);

    scl_coroutine_t *curr = scl_coroutine_find(scl_current_coroutine_pid);
    scl_coroutine_t *next = scl_coroutine_find(scl_next_coroutine_pid);
    scl_current_coroutine_pid = scl_next_coroutine_pid;

    if (!curr) {
        swapcontext(&scl_context, &next->ctx);
    }

    if (curr != next) {
        swapcontext(&curr->ctx, &next->ctx);
    }
    return;
}

void scl_coroutine_entry() {
    scl_coroutine_t *c = &scl_coroutines[scl_current_coroutine_pid];
    c->routine();

    c->status = 'c';
    scl_coroutine_yield();
}

uint16_t scl_coroutine_summon(void (*routine)()) {
    uint16_t pid = scl_coroutine_unused_pid();
    scl_coroutine_t *c = &scl_coroutines[pid];

    c->pid     = pid;
    c->status  = 'r';
    c->wake_at = 0;
    c->stack   = malloc(STACK_SIZE);
    c->routine = routine;

    // config ctx
    getcontext(&c->ctx);

    c->ctx.uc_stack.ss_sp = c->stack;
    c->ctx.uc_stack.ss_size = STACK_SIZE;
    c->ctx.uc_link = NULL;

    makecontext(&c->ctx, scl_coroutine_entry, 0);
    return c->pid;
}

void scl_coroutine_init() {
    if (setjmp(scl_context_jmp) != 0) {
        scl_current_coroutine_pid = 0xFFFF;
        scl_coroutine_gc();
        return;
    }
    
    getcontext(&scl_context);

    if (!scl_coroutine_inited) {
        scl_coroutine_inited = true;
        scl_coroutine_yield();
    }
}

void scl_coroutine_sleep(uint64_t ms) {
    scl_coroutine_t *c = &scl_coroutines[scl_current_coroutine_pid];
    c->wake_at = scl_ms() + ms;
    c->status = 's';
    scl_coroutine_yield();
}

#endif