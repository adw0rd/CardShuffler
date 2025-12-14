#include "graphics.h"
#include "st7735.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>

// Basic 5x7 font
static const uint8_t font5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // Space
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x08, 0x2A, 0x1C, 0x2A, 0x08, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x01, 0x01, // F
    0x3E, 0x41, 0x41, 0x51, 0x32, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x04, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x7F, 0x20, 0x18, 0x20, 0x7F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x03, 0x04, 0x78, 0x04, 0x03, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // backslash
    0x41, 0x41, 0x7F, 0x00, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x08, 0x14, 0x54, 0x54, 0x3C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x00, 0x7F, 0x10, 0x28, 0x44, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x08, 0x08, 0x2A, 0x1C, 0x08, // ->
    0x08, 0x1C, 0x2A, 0x08, 0x08, // <-
};

void gfx_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    gfx_draw_line(x, y, x + w - 1, y, color);
    gfx_draw_line(x, y + h - 1, x + w - 1, y + h - 1, color);
    gfx_draw_line(x, y, x, y + h - 1, color);
    gfx_draw_line(x + w - 1, y, x + w - 1, y + h - 1, color);
}

void gfx_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    int16_t dx, dy, err, ystep;

    if (steep) {
        uint16_t tmp = x0; x0 = y0; y0 = tmp;
        tmp = x1; x1 = y1; y1 = tmp;
    }

    if (x0 > x1) {
        uint16_t tmp = x0; x0 = x1; x1 = tmp;
        tmp = y0; y0 = y1; y1 = tmp;
    }

    dx = x1 - x0;
    dy = abs(y1 - y0);
    err = dx / 2;
    ystep = (y0 < y1) ? 1 : -1;

    for (; x0 <= x1; x0++) {
        if (steep) {
            st7735_draw_pixel(y0, x0, color);
        } else {
            st7735_draw_pixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void gfx_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    st7735_draw_pixel(x0, y0 + r, color);
    st7735_draw_pixel(x0, y0 - r, color);
    st7735_draw_pixel(x0 + r, y0, color);
    st7735_draw_pixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        st7735_draw_pixel(x0 + x, y0 + y, color);
        st7735_draw_pixel(x0 - x, y0 + y, color);
        st7735_draw_pixel(x0 + x, y0 - y, color);
        st7735_draw_pixel(x0 - x, y0 - y, color);
        st7735_draw_pixel(x0 + y, y0 + x, color);
        st7735_draw_pixel(x0 - y, y0 + x, color);
        st7735_draw_pixel(x0 + y, y0 - x, color);
        st7735_draw_pixel(x0 - y, y0 - x, color);
    }
}

void gfx_fill_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    st7735_fill_rect(x0 - r, y0, 2 * r + 1, 1, color);

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        st7735_fill_rect(x0 - x, y0 + y, 2 * x + 1, 1, color);
        st7735_fill_rect(x0 - x, y0 - y, 2 * x + 1, 1, color);
        st7735_fill_rect(x0 - y, y0 + x, 2 * y + 1, 1, color);
        st7735_fill_rect(x0 - y, y0 - x, 2 * y + 1, 1, color);
    }
}

static void gfx_fill_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (corners & 0x1) {
            st7735_fill_rect(x0 - y, y0 - x, 2 * y + delta + 1, 1, color);
            st7735_fill_rect(x0 - x, y0 - y, 2 * x + delta + 1, 1, color);
        }
        if (corners & 0x2) {
            st7735_fill_rect(x0 - y, y0 + x, 2 * y + delta + 1, 1, color);
            st7735_fill_rect(x0 - x, y0 + y, 2 * x + delta + 1, 1, color);
        }
    }
}

void gfx_draw_rounded_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color) {
    gfx_draw_line(x + r, y, x + w - r - 1, y, color);
    gfx_draw_line(x + r, y + h - 1, x + w - r - 1, y + h - 1, color);
    gfx_draw_line(x, y + r, x, y + h - r - 1, color);
    gfx_draw_line(x + w - 1, y + r, x + w - 1, y + h - r - 1, color);

    // Corners
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t cx = 0;
    int16_t cy = r;

    while (cx < cy) {
        if (f >= 0) {
            cy--;
            ddF_y += 2;
            f += ddF_y;
        }
        cx++;
        ddF_x += 2;
        f += ddF_x;

        // Top-right
        st7735_draw_pixel(x + w - 1 - r + cx, y + r - cy, color);
        st7735_draw_pixel(x + w - 1 - r + cy, y + r - cx, color);
        // Top-left
        st7735_draw_pixel(x + r - cx, y + r - cy, color);
        st7735_draw_pixel(x + r - cy, y + r - cx, color);
        // Bottom-right
        st7735_draw_pixel(x + w - 1 - r + cx, y + h - 1 - r + cy, color);
        st7735_draw_pixel(x + w - 1 - r + cy, y + h - 1 - r + cx, color);
        // Bottom-left
        st7735_draw_pixel(x + r - cx, y + h - 1 - r + cy, color);
        st7735_draw_pixel(x + r - cy, y + h - 1 - r + cx, color);
    }
}

void gfx_fill_rounded_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color) {
    st7735_fill_rect(x + r, y, w - 2 * r, h, color);
    gfx_fill_circle_helper(x + r, y + r, r, 1, h - 2 * r - 1, color);
    gfx_fill_circle_helper(x + w - r - 1, y + r, r, 2, h - 2 * r - 1, color);
}

void gfx_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
    if (c < 32 || c > 127) c = '?';

    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = font5x7[(c - 32) * 5 + i];
        for (uint8_t j = 0; j < 8; j++) {
            if (line & 0x1) {
                if (size == 1) {
                    st7735_draw_pixel(x + i, y + j, color);
                } else {
                    st7735_fill_rect(x + i * size, y + j * size, size, size, color);
                }
            } else if (bg != color) {
                if (size == 1) {
                    st7735_draw_pixel(x + i, y + j, bg);
                } else {
                    st7735_fill_rect(x + i * size, y + j * size, size, size, bg);
                }
            }
            line >>= 1;
        }
    }
}

void gfx_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size) {
    while (*str) {
        gfx_draw_char(x, y, *str++, color, bg, size);
        x += 6 * size;
    }
}

void gfx_draw_string_centered(uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size) {
    uint16_t len = strlen(str);
    uint16_t x = (DISPLAY_WIDTH - len * 6 * size) / 2;
    gfx_draw_string(x, y, str, color, bg, size);
}

void gfx_draw_bitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, uint16_t bg) {
    uint16_t byteWidth = (w + 7) / 8;
    for (uint16_t j = 0; j < h; j++) {
        for (uint16_t i = 0; i < w; i++) {
            if (bitmap[j * byteWidth + i / 8] & (0x80 >> (i % 8))) {
                st7735_draw_pixel(x + i, y + j, color);
            } else {
                st7735_draw_pixel(x + i, y + j, bg);
            }
        }
    }
}

void gfx_draw_bitmap_transparent(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color) {
    uint16_t byteWidth = (w + 7) / 8;
    for (uint16_t j = 0; j < h; j++) {
        for (uint16_t i = 0; i < w; i++) {
            if (bitmap[j * byteWidth + i / 8] & (0x80 >> (i % 8))) {
                st7735_draw_pixel(x + i, y + j, color);
            }
        }
    }
}
