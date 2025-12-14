#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>

void gfx_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void gfx_draw_rounded_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
void gfx_fill_rounded_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
void gfx_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void gfx_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void gfx_fill_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void gfx_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
void gfx_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size);
void gfx_draw_string_centered(uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size);
void gfx_draw_bitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, uint16_t bg);
void gfx_draw_bitmap_transparent(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);

#endif // GRAPHICS_H
