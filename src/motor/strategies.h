#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <stdint.h>
#include <stdbool.h>
#include "ui/menu.h"

void strategy_run(strategy_id_t strategy);
void strategy_stop(void);
bool strategy_is_running(void);
uint8_t strategy_get_progress(void);  // 0-100%
uint16_t strategy_get_remaining_sec(void);  // seconds remaining

#endif // STRATEGIES_H
