#include "motor.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdlib.h>

static uint8_t current_speed[2] = {0, 0};
static uint slice_num[2];
static uint channel[2];

// PWM wrap value for ~25kHz at 125MHz system clock
#define PWM_WRAP 5000

static uint8_t motor_pin(motor_id_t motor) {
    return (motor == MOTOR_1) ? PIN_MOTOR1 : PIN_MOTOR2;
}

void motor_init(void) {
    // Configure PWM for both motors
    uint8_t pins[] = {PIN_MOTOR1, PIN_MOTOR2};

    for (int i = 0; i < 2; i++) {
        gpio_set_function(pins[i], GPIO_FUNC_PWM);
        slice_num[i] = pwm_gpio_to_slice_num(pins[i]);
        channel[i] = pwm_gpio_to_channel(pins[i]);

        pwm_set_wrap(slice_num[i], PWM_WRAP);
        pwm_set_chan_level(slice_num[i], channel[i], 0);
        pwm_set_enabled(slice_num[i], true);

        current_speed[i] = 0;
    }
}

void motor_set_speed(motor_id_t motor, uint8_t speed) {
    if (speed > 100) speed = 100;

    uint16_t level = (PWM_WRAP * speed) / 100;
    pwm_set_chan_level(slice_num[motor], channel[motor], level);
    current_speed[motor] = speed;
}

void motor_set_speed_both(uint8_t speed1, uint8_t speed2) {
    motor_set_speed(MOTOR_1, speed1);
    motor_set_speed(MOTOR_2, speed2);
}

void motor_stop(motor_id_t motor) {
    motor_set_speed(motor, 0);
}

void motor_stop_all(void) {
    motor_set_speed(MOTOR_1, 0);
    motor_set_speed(MOTOR_2, 0);
}

void motor_ramp_to(motor_id_t motor, uint8_t target_speed, uint16_t duration_ms) {
    if (target_speed > 100) target_speed = 100;

    int16_t start = current_speed[motor];
    int16_t diff = (int16_t)target_speed - start;

    if (diff == 0 || duration_ms == 0) {
        motor_set_speed(motor, target_speed);
        return;
    }

    uint16_t steps = duration_ms / 10;  // Update every 10ms
    if (steps == 0) steps = 1;

    for (uint16_t i = 1; i <= steps; i++) {
        int16_t new_speed = start + (diff * i) / steps;
        motor_set_speed(motor, (uint8_t)new_speed);
        sleep_ms(10);
    }
}

void motor_ramp_both(uint8_t target1, uint8_t target2, uint16_t duration_ms) {
    if (target1 > 100) target1 = 100;
    if (target2 > 100) target2 = 100;

    int16_t start1 = current_speed[0];
    int16_t start2 = current_speed[1];
    int16_t diff1 = (int16_t)target1 - start1;
    int16_t diff2 = (int16_t)target2 - start2;

    if ((diff1 == 0 && diff2 == 0) || duration_ms == 0) {
        motor_set_speed_both(target1, target2);
        return;
    }

    uint16_t steps = duration_ms / 10;
    if (steps == 0) steps = 1;

    for (uint16_t i = 1; i <= steps; i++) {
        int16_t new1 = start1 + (diff1 * i) / steps;
        int16_t new2 = start2 + (diff2 * i) / steps;
        motor_set_speed_both((uint8_t)new1, (uint8_t)new2);
        sleep_ms(10);
    }
}

uint8_t motor_get_speed(motor_id_t motor) {
    return current_speed[motor];
}

bool motor_is_running(void) {
    return (current_speed[0] > 0 || current_speed[1] > 0);
}
