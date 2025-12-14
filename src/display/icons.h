#ifndef ICONS_H
#define ICONS_H

#include <stdint.h>

#define ICON_WIDTH  24
#define ICON_HEIGHT 24

typedef enum {
    ICON_SPADE,
    ICON_HEART,
    ICON_DIAMOND,
    ICON_CLUB,
    ICON_CARDS,
    ICON_GEAR,
    ICON_COUNT
} icon_id_t;

extern const uint8_t icon_spade[];
extern const uint8_t icon_heart[];
extern const uint8_t icon_diamond[];
extern const uint8_t icon_club[];
extern const uint8_t icon_cards[];
extern const uint8_t icon_gear[];

const uint8_t* icon_get(icon_id_t id);

#endif // ICONS_H
