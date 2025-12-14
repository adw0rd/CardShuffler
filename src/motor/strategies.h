#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <stdint.h>
#include <stdbool.h>
#include "ui/menu.h"

typedef struct {
    uint8_t motor1_speed;   // 0-100
    uint8_t motor2_speed;   // 0-100
    uint16_t duration_ms;   // Total shuffle duration
    uint16_t pulse_on_ms;   // Pulse on time (for pulsed strategies)
    uint16_t pulse_off_ms;  // Pulse off time
    bool alternating;       // Alternate between motors
} custom_params_t;

void strategy_run(strategy_id_t strategy);
void strategy_stop(void);
bool strategy_is_running(void);
uint8_t strategy_get_progress(void);  // 0-100%

// Custom strategy configuration
void strategy_set_custom(const custom_params_t *params);
custom_params_t* strategy_get_custom(void);

#endif // STRATEGIES_H
