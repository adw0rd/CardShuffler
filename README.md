# Card Shuffler

Firmware for an automatic card shuffler ([Ali](https://aliexpress.ru/item/1005009151783502.html), [Oz](https://www.ozon.ru/product/shafl-mashinka-dlya-peremeshivaniya-kart-2946931123/), [Oz](https://www.ozon.ru/product/avtomaticheskiy-peremeshivatel-kart-dlya-pokera-elektronnyy-peremeshivatel-2022-chernyy-3070517864/), [Oz](https://www.ozon.ru/product/ustroystvo-dlya-peremeshivaniya-kart-1807983264/)) powered by RP2040 (Raspberry Pi Pico) with an TFT display (ST7735) and rotary encoder (KY-040) navigation.

<img width="200" height="200" alt="cf" src="https://github.com/user-attachments/assets/eafe9d9e-fd7c-4673-bf4b-0709df51f572" />
<img width="200" height="200" alt="st7735" src="https://github.com/user-attachments/assets/4f1d5d8a-cf32-44b7-8409-d0241e23001b" />
<img width="200" height="200" alt="rp2040" src="https://github.com/user-attachments/assets/be653af1-b808-4f67-8b3a-fc369c410fa8" />
<img width="200" height="200" alt="ky040" src="https://github.com/user-attachments/assets/6717d590-b0b0-4bbc-8fef-1ebbf12ee844" />

## Features

- **6 Shuffling Strategies**
  - **Quick** — Both motors at full speed
  - **Riffle** — Fast alternating pulses between motors
  - **Strip** — Short bursts from one motor while other runs slow
  - **Wash** — Random chaotic mixing pattern
  - **Box** — Systematic alternating speed phases
  - **Custom** — User-configurable parameters

- **Visual Menu** with card suit icons (♠♥♦♣)
- **Rotary Encoder** navigation with button to start/stop
- **PWM Motor Control** with soft start/stop ramping
- **Progress Bar** during shuffling

## Hardware

| Component | Connection |
|-----------|------------|
| ST7735 Display | SPI0 (GPIO 17-22) |
| Rotary Encoder | GPIO 14-16 |
| Motor 1 | GPIO 2 (PWM) |
| Motor 2 | GPIO 3 (PWM) |

### Pinout

```
Display:        Encoder:        Motors:
MOSI  → GP19    A   → GP14      M1 → GP2
SCK   → GP18    B   → GP15      M2 → GP3
CS    → GP17    BTN → GP16
DC    → GP20
RST   → GP21
BL    → GP22
```

## Building

Requires [Pico SDK](https://github.com/raspberrypi/pico-sdk). The SDK will be fetched automatically during build.

```bash
mkdir build && cd build
cmake ..
make -j4
```

## Flashing

1. Hold **BOOTSEL** button on Pico
2. Connect USB cable
3. Copy `build/card_shuffler.uf2` to the mounted `RPI-RP2` drive
4. Pico will reboot and run the firmware

## Usage

1. **Rotate encoder** to select a shuffling strategy
2. **Press encoder** to start shuffling
3. **Press again** to stop early (motors will ramp down smoothly)

## Configuration

Edit `src/config.h` to change:
- Pin assignments
- Display dimensions
- Motor PWM frequency
- Timing constants
- UI colors (RGB565 format)

Edit `src/motor/strategies.c` to customize:
- Strategy durations
- Motor speeds
- Pulse timings

## Project Structure

```
src/
├── main.c              # Application entry point and state machine
├── config.h            # Hardware configuration
├── display/
│   ├── st7735.c/h      # Display driver
│   ├── graphics.c/h    # Drawing primitives and font
│   └── icons.c/h       # 24x24 card suit bitmaps
├── ui/
│   └── menu.c/h        # Menu system with icon grid
├── input/
│   └── encoder.c/h     # Rotary encoder with debouncing
└── motor/
    ├── motor.c/h       # PWM motor control
    └── strategies.c/h  # Shuffling algorithms
```

## License

MIT
