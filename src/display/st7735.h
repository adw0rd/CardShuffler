#ifndef ST7735_H
#define ST7735_H

#include <stdint.h>
#include <stdbool.h>

void st7735_init(void);
void st7735_set_rotation(uint8_t rotation);
void st7735_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void st7735_fill_screen(uint16_t color);
void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void st7735_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void st7735_write_data(const uint8_t *data, size_t len);
void st7735_write_data16(const uint16_t *data, size_t len);
void st7735_backlight(bool on);

#endif // ST7735_H
