# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Card Shuffler firmware for RP2040-Zero with ST7735 display, rotary encoder navigation, TB6612FNG motor driver, and dual PWM motor control.

## Build Commands

```bash
make          # build firmware
make clean    # clean build
make flash    # build and flash (if RPI-RP2 mounted)
```

Flash by holding BOOT on RP2040-Zero while connecting USB (or tap RESET while holding BOOT).

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

## Pin Configuration (config.h) - RP2040-Zero

| Function | GPIO |
|----------|------|
| Display MOSI | 7 |
| Display SCK | 6 |
| Display CS | 5 |
| Display DC | 8 |
| Display RST | 9 |
| Display BL | 10 |
| Encoder A | 14 |
| Encoder B | 15 |
| Encoder BTN | 16 |
| Extra Button | 4 |
| Motor 1 (PWMA) | 2 |
| Motor 2 (PWMB) | 3 |

## Key Patterns

- Colors are RGB565 format (16-bit)
- Encoder uses interrupt-driven state machine with 4-step detent (divide delta by 4)
- Motor PWM wrap value 5000 for ~25kHz at 125MHz
- Strategies block during execution, check `running` flag in loops for cancellation
