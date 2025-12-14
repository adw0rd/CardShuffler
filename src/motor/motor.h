#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    MOTOR_1 = 0,
    MOTOR_2 = 1
} motor_id_t;

void motor_init(void);
void motor_set_speed(motor_id_t motor, uint8_t speed);  // 0-100%
void motor_set_speed_both(uint8_t speed1, uint8_t speed2);
void motor_stop(motor_id_t motor);
void motor_stop_all(void);
void motor_ramp_to(motor_id_t motor, uint8_t target_speed, uint16_t duration_ms);
void motor_ramp_both(uint8_t target1, uint8_t target2, uint16_t duration_ms);
uint8_t motor_get_speed(motor_id_t motor);
bool motor_is_running(void);

#endif // MOTOR_H
