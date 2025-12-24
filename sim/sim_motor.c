// Motor stub for simulator
#include "motor/motor.h"
#include <stdio.h>

static uint8_t motor_speeds[2] = {0, 0};
static bool running = false;

void motor_init(void) {
    motor_speeds[0] = 0;
    motor_speeds[1] = 0;
    running = false;
}

void motor_set_speed(motor_id_t motor, uint8_t speed) {
    if (motor > 1) return;
    motor_speeds[motor] = speed;
    running = (motor_speeds[0] > 0 || motor_speeds[1] > 0);
    printf("[Motor %d] Speed: %d%%\n", motor, speed);
}

void motor_set_speed_both(uint8_t speed1, uint8_t speed2) {
    motor_speeds[0] = speed1;
    motor_speeds[1] = speed2;
    running = (speed1 > 0 || speed2 > 0);
    printf("[Motors] Speed: %d%%, %d%%\n", speed1, speed2);
}

void motor_stop(motor_id_t motor) {
    if (motor > 1) return;
    motor_speeds[motor] = 0;
    running = (motor_speeds[0] > 0 || motor_speeds[1] > 0);
    printf("[Motor %d] Stopped\n", motor);
}

void motor_stop_all(void) {
    motor_speeds[0] = 0;
    motor_speeds[1] = 0;
    running = false;
    printf("[Motors] All stopped\n");
}

void motor_ramp_to(motor_id_t motor, uint8_t target_speed, uint16_t duration_ms) {
    (void)duration_ms;
    motor_set_speed(motor, target_speed);
}

void motor_ramp_both(uint8_t target1, uint8_t target2, uint16_t duration_ms) {
    (void)duration_ms;
    motor_set_speed_both(target1, target2);
}

uint8_t motor_get_speed(motor_id_t motor) {
    if (motor > 1) return 0;
    return motor_speeds[motor];
}

bool motor_is_running(void) {
    return running;
}
