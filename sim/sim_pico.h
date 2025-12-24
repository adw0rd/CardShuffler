// Pico SDK stubs for simulator
#ifndef SIM_PICO_H
#define SIM_PICO_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Time functions - implemented in sim_main.c
extern void sim_sleep_ms(uint32_t ms);
extern uint32_t sim_time_us(void);

// Stub for pico/stdlib.h
static inline void stdio_init_all(void) {}

static inline void sleep_ms(uint32_t ms) {
    sim_sleep_ms(ms);
}

static inline void sleep_us(uint32_t us) {
    sim_sleep_ms(us / 1000);
}

// Pico time types
typedef uint64_t absolute_time_t;

static inline absolute_time_t get_absolute_time(void) {
    return (absolute_time_t)sim_time_us();
}

static inline uint32_t to_ms_since_boot(absolute_time_t t) {
    return (uint32_t)(t / 1000);
}

static inline uint32_t time_us_32(void) {
    return sim_time_us();
}

#endif // SIM_PICO_H
