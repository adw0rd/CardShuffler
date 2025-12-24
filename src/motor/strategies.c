#include "strategies.h"
#include "motor.h"
#include "config.h"
#include "ui/menu.h"
#include "display/st7735.h"
#include "input/encoder.h"
#include "pico/stdlib.h"
#include <stdlib.h>

static bool running = false;
static uint32_t start_time = 0;
static uint32_t total_duration = 0;
static strategy_id_t current_strategy = STRATEGY_QUICK;
static uint32_t stop_time = 0;  // When strategy was stopped

// Sleep with display update and button check
static void strategy_sleep_ms(uint32_t ms) {
    uint32_t step = 50;  // Check every 50ms
    while (ms > 0 && running) {
        uint32_t delay = (ms > step) ? step : ms;
        sleep_ms(delay);
        ms -= delay;

        // Check for stop button
        encoder_event_t event = encoder_poll();
        if (event == ENC_EVENT_PRESS || event == ENC_EVENT_RELEASE) {
            running = false;
            break;
        }

        // Update display
        menu_draw_shuffling(strategy_get_progress(), strategy_get_remaining_sec());
        st7735_flush();
    }
}

// Strategy: Quick - Both motors full speed
static void run_quick(void) {
    total_duration = 5000;
    motor_ramp_both(100, 100, MOTOR_RAMP_MS);

    while (running) {
        uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - start_time;
        if (elapsed >= total_duration) break;
        strategy_sleep_ms(50);
    }

    motor_ramp_both(0, 0, MOTOR_RAMP_MS);
}

// Strategy: Riffle - Quick alternating pulses
static void run_riffle(void) {
    total_duration = 6000;
    uint16_t pulse_ms = 150;
    uint16_t gap_ms = 50;

    while (running) {
        uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - start_time;
        if (elapsed >= total_duration) break;

        // Motor 1 pulse
        motor_set_speed(MOTOR_1, 100);
        strategy_sleep_ms(pulse_ms);
        motor_set_speed(MOTOR_1, 0);
        strategy_sleep_ms(gap_ms);

        if (!running) break;

        // Motor 2 pulse
        motor_set_speed(MOTOR_2, 100);
        strategy_sleep_ms(pulse_ms);
        motor_set_speed(MOTOR_2, 0);
        strategy_sleep_ms(gap_ms);
    }

    motor_stop_all();
}

// Strategy: Strip - Short bursts from one motor
static void run_strip(void) {
    total_duration = 5000;
    uint16_t burst_ms = 300;
    uint16_t gap_ms = 400;

    motor_ramp_to(MOTOR_2, 50, MOTOR_RAMP_MS);  // Slow base rotation

    while (running) {
        uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - start_time;
        if (elapsed >= total_duration) break;

        // Quick burst from motor 1
        motor_set_speed(MOTOR_1, 100);
        strategy_sleep_ms(burst_ms);
        motor_set_speed(MOTOR_1, 30);
        strategy_sleep_ms(gap_ms);
    }

    motor_ramp_both(0, 0, MOTOR_RAMP_MS);
}

// Strategy: Wash - Random chaotic mixing
static void run_wash(void) {
    total_duration = 8000;

    while (running) {
        uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - start_time;
        if (elapsed >= total_duration) break;

        // Random speeds and durations
        uint8_t speed1 = 40 + (rand() % 60);  // 40-100
        uint8_t speed2 = 40 + (rand() % 60);
        uint16_t hold_ms = 100 + (rand() % 300);

        motor_set_speed_both(speed1, speed2);
        strategy_sleep_ms(hold_ms);
    }

    motor_ramp_both(0, 0, MOTOR_RAMP_MS);
}

// Strategy: Box - Systematic alternating
static void run_box(void) {
    total_duration = 6000;
    uint16_t phase_ms = 800;

    while (running) {
        uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - start_time;
        if (elapsed >= total_duration) break;

        // Phase 1: Motor 1 fast, Motor 2 slow
        motor_ramp_both(90, 30, 100);
        strategy_sleep_ms(phase_ms);
        if (!running) break;

        // Phase 2: Both medium
        motor_ramp_both(60, 60, 100);
        strategy_sleep_ms(phase_ms / 2);
        if (!running) break;

        // Phase 3: Motor 1 slow, Motor 2 fast
        motor_ramp_both(30, 90, 100);
        strategy_sleep_ms(phase_ms);
        if (!running) break;

        // Phase 4: Both medium
        motor_ramp_both(60, 60, 100);
        strategy_sleep_ms(phase_ms / 2);
    }

    motor_ramp_both(0, 0, MOTOR_RAMP_MS);
}

// Strategy: Random - randomly pick another strategy
static void run_random(void) {
    // Pick random strategy (0-4, excluding STRATEGY_RANDOM itself)
    strategy_id_t pick = (strategy_id_t)(rand() % 5);

    switch (pick) {
        case STRATEGY_QUICK:  run_quick();  break;
        case STRATEGY_RIFFLE: run_riffle(); break;
        case STRATEGY_STRIP:  run_strip();  break;
        case STRATEGY_WASH:   run_wash();   break;
        case STRATEGY_BOX:    run_box();    break;
        default:              run_quick();  break;
    }
}

void strategy_run(strategy_id_t strategy) {
    if (running) return;

    // Prevent immediate restart after stop (debounce 500ms)
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (stop_time > 0 && (now - stop_time) < 500) return;

    running = true;
    start_time = now;
    current_strategy = strategy;

    switch (strategy) {
        case STRATEGY_QUICK:
            run_quick();
            break;
        case STRATEGY_RIFFLE:
            run_riffle();
            break;
        case STRATEGY_STRIP:
            run_strip();
            break;
        case STRATEGY_WASH:
            run_wash();
            break;
        case STRATEGY_BOX:
            run_box();
            break;
        case STRATEGY_RANDOM:
            run_random();
            break;
        default:
            run_quick();
            break;
    }

    running = false;
    stop_time = to_ms_since_boot(get_absolute_time());
    motor_stop_all();
}

void strategy_stop(void) {
    running = false;
    stop_time = to_ms_since_boot(get_absolute_time());
    motor_stop_all();
}

bool strategy_is_running(void) {
    return running;
}

uint8_t strategy_get_progress(void) {
    if (!running || total_duration == 0) return 0;

    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - start_time;
    if (elapsed >= total_duration) return 100;

    return (uint8_t)((elapsed * 100) / total_duration);
}

uint16_t strategy_get_remaining_sec(void) {
    if (!running || total_duration == 0) return 0;

    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - start_time;
    if (elapsed >= total_duration) return 0;

    return (uint16_t)((total_duration - elapsed) / 1000);
}
