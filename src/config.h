#ifndef CONFIG_H
#define CONFIG_H

// =============================================================================
// Display ST7735 (SPI0) - RP2040-Zero pinout
// =============================================================================
#define PIN_DISPLAY_MOSI    7
#define PIN_DISPLAY_SCK     6
#define PIN_DISPLAY_CS      5
#define PIN_DISPLAY_DC      8
#define PIN_DISPLAY_RST     9
#define PIN_DISPLAY_BL      10

#define DISPLAY_SPI         spi0
#define DISPLAY_BAUDRATE    (20000000)  // 20 MHz (stable for long wires)

#define DISPLAY_WIDTH       128
#define DISPLAY_HEIGHT      160
#define DISPLAY_ROTATION    1   // 0=0°, 1=90°, 2=180°, 3=270°

// Green tab display offsets
#define DISPLAY_OFFSET_X    2
#define DISPLAY_OFFSET_Y    1

// =============================================================================
// Rotary Encoder
// =============================================================================
#define PIN_ENC_A           14
#define PIN_ENC_B           15
#define PIN_ENC_BTN         16

// Extra button (directly on enclosure)
#define PIN_EXTRA_BTN       4

#define ENCODER_DEBOUNCE_MS 5

// =============================================================================
// Motors (PWM)
// =============================================================================
#define PIN_MOTOR1          2
#define PIN_MOTOR2          3

#define MOTOR_PWM_FREQ      25000   // 25kHz - silent operation
#define MOTOR_RAMP_MS       200     // Soft start/stop duration

// =============================================================================
// Menu Configuration
// =============================================================================
#define MENU_COLS           3
#define MENU_ROWS           2
#define ICON_SIZE           32
#define ICON_SPACING        8

// =============================================================================
// Strategy Timing (ms)
// =============================================================================
#define SHUFFLE_DEFAULT_DURATION    5000
#define SHUFFLE_MIN_DURATION        1000
#define SHUFFLE_MAX_DURATION        30000

// =============================================================================
// Colors (RGB565)
// =============================================================================
#define COLOR_BLACK         0x0000
#define COLOR_WHITE         0xFFFF
#define COLOR_RED           0xF800
#define COLOR_GREEN         0x07E0
#define COLOR_BLUE          0x001F
#define COLOR_YELLOW        0xFFE0
#define COLOR_CYAN          0x07FF
#define COLOR_MAGENTA       0xF81F

#define COLOR_DARK_GRAY     0x4208
#define COLOR_LIGHT_GRAY    0xC618

#define COLOR_CARD_RED      0xF800
#define COLOR_CARD_BLACK    0x0000
#define COLOR_CARD_BG       0xFFFF
#define COLOR_MENU_BG       0x2104  // Dark blue-gray
#define COLOR_HIGHLIGHT     0x07FF  // Cyan

#endif // CONFIG_H
