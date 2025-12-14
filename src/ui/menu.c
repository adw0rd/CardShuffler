#include "menu.h"
#include "config.h"
#include "display/st7735.h"
#include "display/graphics.h"
#include "display/icons.h"
#include <string.h>

#define TITLE_HEIGHT    20
#define GRID_MARGIN_X   8
#define GRID_MARGIN_Y   28
#define CELL_WIDTH      36
#define CELL_HEIGHT     44
#define ICON_OFFSET_X   6
#define ICON_OFFSET_Y   4
#define NAME_HEIGHT     20
#define STATUS_HEIGHT   12

static const menu_item_t menu_items[STRATEGY_COUNT] = {
    {"Quick",  ICON_SPADE,   COLOR_CARD_BLACK},
    {"Riffle", ICON_HEART,   COLOR_CARD_RED},
    {"Strip",  ICON_DIAMOND, COLOR_CARD_RED},
    {"Wash",   ICON_CLUB,    COLOR_CARD_BLACK},
    {"Box",    ICON_CARDS,   COLOR_BLUE},
    {"Custom", ICON_GEAR,    COLOR_LIGHT_GRAY},
};

static uint8_t selected_index = 0;
static bool is_shuffling = false;

static void draw_item(uint8_t index, bool highlighted) {
    uint8_t col = index % MENU_COLS;
    uint8_t row = index / MENU_COLS;

    uint16_t x = GRID_MARGIN_X + col * CELL_WIDTH;
    uint16_t y = GRID_MARGIN_Y + row * CELL_HEIGHT;

    // Card background
    uint16_t bg_color = highlighted ? COLOR_HIGHLIGHT : COLOR_CARD_BG;
    uint16_t border_color = highlighted ? COLOR_WHITE : COLOR_DARK_GRAY;

    gfx_fill_rounded_rect(x, y, CELL_WIDTH - 4, CELL_HEIGHT - 8, 4, bg_color);
    gfx_draw_rounded_rect(x, y, CELL_WIDTH - 4, CELL_HEIGHT - 8, 4, border_color);

    // Icon
    const uint8_t *icon = icon_get(menu_items[index].icon);
    uint16_t icon_x = x + ICON_OFFSET_X;
    uint16_t icon_y = y + ICON_OFFSET_Y;

    uint16_t icon_color = menu_items[index].icon_color;
    if (highlighted) {
        icon_color = COLOR_WHITE;
    }

    gfx_draw_bitmap(icon_x, icon_y, icon, ICON_WIDTH, ICON_HEIGHT, icon_color, bg_color);
}

static void draw_name_bar(void) {
    uint16_t y = DISPLAY_HEIGHT - NAME_HEIGHT - STATUS_HEIGHT;

    // Clear name area
    st7735_fill_rect(0, y, DISPLAY_WIDTH, NAME_HEIGHT, COLOR_MENU_BG);

    // Draw strategy name centered
    const char *name = menu_items[selected_index].name;
    gfx_draw_string_centered(y + 6, name, COLOR_WHITE, COLOR_MENU_BG, 1);
}

void menu_init(void) {
    selected_index = 0;
    is_shuffling = false;
}

void menu_draw(void) {
    // Fill background
    st7735_fill_screen(COLOR_MENU_BG);

    // Title
    menu_draw_title("CARD SHUFFLER");

    // Draw all items
    for (uint8_t i = 0; i < STRATEGY_COUNT; i++) {
        draw_item(i, i == selected_index);
    }

    // Strategy name
    draw_name_bar();

    // Status bar
    uint16_t status_y = DISPLAY_HEIGHT - STATUS_HEIGHT;
    st7735_fill_rect(0, status_y, DISPLAY_WIDTH, STATUS_HEIGHT, COLOR_DARK_GRAY);
    gfx_draw_string_centered(status_y + 2, "Press to start", COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, 1);
}

void menu_next(void) {
    if (is_shuffling) return;

    uint8_t old_index = selected_index;
    selected_index = (selected_index + 1) % STRATEGY_COUNT;

    draw_item(old_index, false);
    draw_item(selected_index, true);
    draw_name_bar();
}

void menu_prev(void) {
    if (is_shuffling) return;

    uint8_t old_index = selected_index;
    selected_index = (selected_index == 0) ? STRATEGY_COUNT - 1 : selected_index - 1;

    draw_item(old_index, false);
    draw_item(selected_index, true);
    draw_name_bar();
}

strategy_id_t menu_get_selected(void) {
    return (strategy_id_t)selected_index;
}

void menu_set_shuffling(bool active) {
    is_shuffling = active;

    uint16_t status_y = DISPLAY_HEIGHT - STATUS_HEIGHT;
    st7735_fill_rect(0, status_y, DISPLAY_WIDTH, STATUS_HEIGHT, COLOR_DARK_GRAY);

    if (active) {
        gfx_draw_string_centered(status_y + 2, "Shuffling...", COLOR_YELLOW, COLOR_DARK_GRAY, 1);
    } else {
        gfx_draw_string_centered(status_y + 2, "Press to start", COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, 1);
    }
}

void menu_draw_progress(uint8_t percent) {
    if (percent > 100) percent = 100;

    uint16_t bar_y = DISPLAY_HEIGHT - STATUS_HEIGHT;
    uint16_t bar_width = (DISPLAY_WIDTH * percent) / 100;

    st7735_fill_rect(0, bar_y, bar_width, 2, COLOR_GREEN);
    st7735_fill_rect(bar_width, bar_y, DISPLAY_WIDTH - bar_width, 2, COLOR_DARK_GRAY);
}

void menu_draw_title(const char *title) {
    st7735_fill_rect(0, 0, DISPLAY_WIDTH, TITLE_HEIGHT, COLOR_DARK_GRAY);
    gfx_draw_string_centered(6, title, COLOR_WHITE, COLOR_DARK_GRAY, 1);
}
