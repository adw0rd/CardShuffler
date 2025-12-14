#include "st7735.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <string.h>

// ST7735 Commands
#define ST7735_NOP      0x00
#define ST7735_SWRESET  0x01
#define ST7735_SLPOUT   0x11
#define ST7735_NORON    0x13
#define ST7735_INVOFF   0x20
#define ST7735_INVON    0x21
#define ST7735_DISPOFF  0x28
#define ST7735_DISPON   0x29
#define ST7735_CASET    0x2A
#define ST7735_RASET    0x2B
#define ST7735_RAMWR    0x2C
#define ST7735_MADCTL   0x36
#define ST7735_COLMOD   0x3A
#define ST7735_FRMCTR1  0xB1
#define ST7735_FRMCTR2  0xB2
#define ST7735_FRMCTR3  0xB3
#define ST7735_INVCTR   0xB4
#define ST7735_PWCTR1   0xC0
#define ST7735_PWCTR2   0xC1
#define ST7735_PWCTR3   0xC2
#define ST7735_PWCTR4   0xC3
#define ST7735_PWCTR5   0xC4
#define ST7735_VMCTR1   0xC5
#define ST7735_GMCTRP1  0xE0
#define ST7735_GMCTRN1  0xE1

// MADCTL bits
#define MADCTL_MY   0x80
#define MADCTL_MX   0x40
#define MADCTL_MV   0x20
#define MADCTL_ML   0x10
#define MADCTL_RGB  0x00
#define MADCTL_BGR  0x08

static uint16_t display_width = DISPLAY_WIDTH;
static uint16_t display_height = DISPLAY_HEIGHT;

static inline void cs_select(void) {
    gpio_put(PIN_DISPLAY_CS, 0);
}

static inline void cs_deselect(void) {
    gpio_put(PIN_DISPLAY_CS, 1);
}

static inline void dc_command(void) {
    gpio_put(PIN_DISPLAY_DC, 0);
}

static inline void dc_data(void) {
    gpio_put(PIN_DISPLAY_DC, 1);
}

static void write_cmd(uint8_t cmd) {
    dc_command();
    cs_select();
    spi_write_blocking(DISPLAY_SPI, &cmd, 1);
    cs_deselect();
}

static void write_data(const uint8_t *data, size_t len) {
    dc_data();
    cs_select();
    spi_write_blocking(DISPLAY_SPI, data, len);
    cs_deselect();
}

static void write_data_byte(uint8_t data) {
    write_data(&data, 1);
}

void st7735_init(void) {
    // Init SPI
    spi_init(DISPLAY_SPI, DISPLAY_BAUDRATE);
    gpio_set_function(PIN_DISPLAY_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_DISPLAY_SCK, GPIO_FUNC_SPI);

    // Init control pins
    gpio_init(PIN_DISPLAY_CS);
    gpio_set_dir(PIN_DISPLAY_CS, GPIO_OUT);
    gpio_put(PIN_DISPLAY_CS, 1);

    gpio_init(PIN_DISPLAY_DC);
    gpio_set_dir(PIN_DISPLAY_DC, GPIO_OUT);

    gpio_init(PIN_DISPLAY_RST);
    gpio_set_dir(PIN_DISPLAY_RST, GPIO_OUT);

    gpio_init(PIN_DISPLAY_BL);
    gpio_set_dir(PIN_DISPLAY_BL, GPIO_OUT);
    gpio_put(PIN_DISPLAY_BL, 1);

    // Hardware reset
    gpio_put(PIN_DISPLAY_RST, 1);
    sleep_ms(10);
    gpio_put(PIN_DISPLAY_RST, 0);
    sleep_ms(10);
    gpio_put(PIN_DISPLAY_RST, 1);
    sleep_ms(120);

    // Init sequence
    write_cmd(ST7735_SWRESET);
    sleep_ms(150);

    write_cmd(ST7735_SLPOUT);
    sleep_ms(500);

    // Frame rate
    write_cmd(ST7735_FRMCTR1);
    write_data((uint8_t[]){0x01, 0x2C, 0x2D}, 3);

    write_cmd(ST7735_FRMCTR2);
    write_data((uint8_t[]){0x01, 0x2C, 0x2D}, 3);

    write_cmd(ST7735_FRMCTR3);
    write_data((uint8_t[]){0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D}, 6);

    // Inversion control
    write_cmd(ST7735_INVCTR);
    write_data_byte(0x07);

    // Power control
    write_cmd(ST7735_PWCTR1);
    write_data((uint8_t[]){0xA2, 0x02, 0x84}, 3);

    write_cmd(ST7735_PWCTR2);
    write_data_byte(0xC5);

    write_cmd(ST7735_PWCTR3);
    write_data((uint8_t[]){0x0A, 0x00}, 2);

    write_cmd(ST7735_PWCTR4);
    write_data((uint8_t[]){0x8A, 0x2A}, 2);

    write_cmd(ST7735_PWCTR5);
    write_data((uint8_t[]){0x8A, 0xEE}, 2);

    // VCOM
    write_cmd(ST7735_VMCTR1);
    write_data_byte(0x0E);

    write_cmd(ST7735_INVOFF);

    // Memory data access control
    write_cmd(ST7735_MADCTL);
    write_data_byte(MADCTL_MX | MADCTL_MY | MADCTL_RGB);

    // 16-bit color
    write_cmd(ST7735_COLMOD);
    write_data_byte(0x05);

    // Gamma
    write_cmd(ST7735_GMCTRP1);
    write_data((uint8_t[]){
        0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D,
        0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10
    }, 16);

    write_cmd(ST7735_GMCTRN1);
    write_data((uint8_t[]){
        0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
        0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10
    }, 16);

    write_cmd(ST7735_NORON);
    sleep_ms(10);

    write_cmd(ST7735_DISPON);
    sleep_ms(100);
}

void st7735_set_rotation(uint8_t rotation) {
    uint8_t madctl;

    switch (rotation % 4) {
        case 0:
            madctl = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
            display_width = DISPLAY_WIDTH;
            display_height = DISPLAY_HEIGHT;
            break;
        case 1:
            madctl = MADCTL_MY | MADCTL_MV | MADCTL_RGB;
            display_width = DISPLAY_HEIGHT;
            display_height = DISPLAY_WIDTH;
            break;
        case 2:
            madctl = MADCTL_RGB;
            display_width = DISPLAY_WIDTH;
            display_height = DISPLAY_HEIGHT;
            break;
        case 3:
            madctl = MADCTL_MX | MADCTL_MV | MADCTL_RGB;
            display_width = DISPLAY_HEIGHT;
            display_height = DISPLAY_WIDTH;
            break;
    }

    write_cmd(ST7735_MADCTL);
    write_data_byte(madctl);
}

void st7735_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    write_cmd(ST7735_CASET);
    write_data((uint8_t[]){0x00, x0, 0x00, x1}, 4);

    write_cmd(ST7735_RASET);
    write_data((uint8_t[]){0x00, y0, 0x00, y1}, 4);

    write_cmd(ST7735_RAMWR);
}

void st7735_fill_screen(uint16_t color) {
    st7735_fill_rect(0, 0, display_width, display_height, color);
}

void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= display_width || y >= display_height) return;

    st7735_set_window(x, y, x, y);
    dc_data();
    cs_select();
    uint8_t data[2] = {color >> 8, color & 0xFF};
    spi_write_blocking(DISPLAY_SPI, data, 2);
    cs_deselect();
}

void st7735_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= display_width || y >= display_height) return;
    if (x + w > display_width) w = display_width - x;
    if (y + h > display_height) h = display_height - y;

    st7735_set_window(x, y, x + w - 1, y + h - 1);

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    dc_data();
    cs_select();

    uint32_t total = (uint32_t)w * h;
    uint8_t buf[64];
    for (int i = 0; i < 32; i++) {
        buf[i * 2] = hi;
        buf[i * 2 + 1] = lo;
    }

    while (total > 0) {
        uint32_t chunk = (total > 32) ? 32 : total;
        spi_write_blocking(DISPLAY_SPI, buf, chunk * 2);
        total -= chunk;
    }

    cs_deselect();
}

void st7735_write_data(const uint8_t *data, size_t len) {
    dc_data();
    cs_select();
    spi_write_blocking(DISPLAY_SPI, data, len);
    cs_deselect();
}

void st7735_write_data16(const uint16_t *data, size_t len) {
    dc_data();
    cs_select();
    for (size_t i = 0; i < len; i++) {
        uint8_t buf[2] = {data[i] >> 8, data[i] & 0xFF};
        spi_write_blocking(DISPLAY_SPI, buf, 2);
    }
    cs_deselect();
}

void st7735_backlight(bool on) {
    gpio_put(PIN_DISPLAY_BL, on ? 1 : 0);
}
