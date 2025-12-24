#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>
#include "display/icons.h"

typedef enum {
    STRATEGY_QUICK,
    STRATEGY_RIFFLE,
    STRATEGY_STRIP,
    STRATEGY_WASH,
    STRATEGY_BOX,
    STRATEGY_RANDOM,
    STRATEGY_COUNT
} strategy_id_t;

typedef struct {
    const char *name;
    icon_id_t icon;
    uint16_t icon_color;
} menu_item_t;

void menu_init(void);
void menu_draw(void);
void menu_next(void);
void menu_prev(void);
strategy_id_t menu_get_selected(void);
void menu_set_shuffling(bool active);
void menu_draw_progress(uint8_t percent);
void menu_draw_title(const char *title);
void menu_draw_shuffling(uint8_t percent, uint16_t remaining_sec);

#endif // MENU_H
