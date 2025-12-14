# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Card Shuffler firmware for RP2040 (Raspberry Pi Pico) with ST7735 display, rotary encoder navigation, and dual PWM motor control.

## Build Commands

```bash
# Create build directory and configure
mkdir build && cd build
cmake ..

# Build firmware
make -j4

# Output: build/card_shuffler.uf2
```

Flash by holding BOOTSEL on Pico while connecting USB, then copy `card_shuffler.uf2` to the mounted drive.

## Architecture

```
src/
├── main.c              # Main loop, state machine (menu/shuffling/stopping)
├── config.h            # Pin definitions, timing constants, colors
├── display/
│   ├── st7735.c/h      # SPI display driver (62.5MHz, RGB565)
│   ├── graphics.c/h    # Drawing primitives, 5x7 font
│   └── icons.c/h       # 24x24 bitmap icons (suits, gear)
├── ui/
│   └── menu.c/h        # Grid menu with 6 strategies, highlight selection
├── input/
│   └── encoder.c/h     # Rotary encoder with interrupt-based state machine
└── motor/
    ├── motor.c/h       # PWM control (25kHz), ramping
    └── strategies.c/h  # Shuffle algorithms (quick, riffle, strip, wash, box, custom)
```

## Pin Configuration (config.h)

| Function | GPIO |
|----------|------|
| Display MOSI | 19 |
| Display SCK | 18 |
| Display CS | 17 |
| Display DC | 20 |
| Display RST | 21 |
| Display BL | 22 |
| Encoder A | 14 |
| Encoder B | 15 |
| Encoder BTN | 16 |
| Motor 1 | 2 |
| Motor 2 | 3 |

## Key Patterns

- Colors are RGB565 format (16-bit)
- Encoder uses interrupt-driven state machine with 4-step detent (divide delta by 4)
- Motor PWM wrap value 5000 for ~25kHz at 125MHz
- Strategies block during execution, check `running` flag in loops for cancellation
