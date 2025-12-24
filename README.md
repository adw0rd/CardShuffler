# Card Shuffler

Firmware for an automatic card shuffler ([Ali](https://aliexpress.ru/item/1005009151783502.html), [Oz](https://www.ozon.ru/product/shafl-mashinka-dlya-peremeshivaniya-kart-2946931123/), [Oz](https://www.ozon.ru/product/avtomaticheskiy-peremeshivatel-kart-dlya-pokera-elektronnyy-peremeshivatel-2022-chernyy-3070517864/), [Oz](https://www.ozon.ru/product/ustroystvo-dlya-peremeshivaniya-kart-1807983264/)) powered by RP2040-Zero with TFT display (ST7735), rotary encoder (KY-040), and TB6612FNG motor driver.

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

| Component | Model | Connection |
|-----------|-------|------------|
| Microcontroller | RP2040-Zero | — |
| Display | ST7735 128x160 | SPI0 (GPIO 5-10) |
| Encoder | KY-040 | GPIO 14-16 |
| Motor Driver | TB6612FNG | GPIO 2-3 (PWM) |

### Pinout (RP2040-Zero)

```
                    USB-C
                   ┌─────┐
              5V ──┤●   ●├── GND
             GND ──┤●   ●├── 3V3 ← VCC (display, encoder, TB6612)
            GP29 ──┤●   ●├── GP0
            GP28 ──┤●   ●├── GP1
            GP27 ──┤●   ●├── GP2 → PWMA (Motor 1)
            GP26 ──┤●   ●├── GP3 → PWMB (Motor 2)
  Encoder A GP15 ──┤●   ●├── GP4
  Encoder B GP14 ──┤●   ●├── GP5 → CS
                   └──┬──┘
              ┌───────┴───────┐
             GP11 GP10 GP9  GP8  GP16
              │    │    │    │    └─→ Encoder BTN
              │    │    │    └──────→ DC
              │    │    └───────────→ RST
              │    └────────────────→ BL
              │
             GP6  GP7  GP12 GP13
              │    └────────────────→ MOSI
              └─────────────────────→ SCK
```

### Wiring

| Function | GPIO | Connect to |
|----------|------|------------|
| Display SCK | GP6 | SCL |
| Display MOSI | GP7 | SDA |
| Display CS | GP5 | CS |
| Display DC | GP8 | DC |
| Display RST | GP9 | RES |
| Display BL | GP10 | BLK |
| Encoder A | GP14 | CLK |
| Encoder B | GP15 | DT |
| Encoder BTN | GP16 | SW |
| Extra Button | GP4 | Start/Stop |
| Motor 1 PWM | GP2 | TB6612 PWMA |
| Motor 2 PWM | GP3 | TB6612 PWMB |

### TB6612FNG Wiring

```
RP2040-Zero          TB6612FNG              Motors
───────────          ─────────              ──────
3V3 ───────────────→ VCC
GND ───────────────→ GND ←───────────────── GND (power supply)
                     VM ←────────────────── +5-12V (power supply)
GP2 ───────────────→ PWMA
3V3 ───────────────→ AIN1                   AOUT1 ──→ Motor 1
GND ───────────────→ AIN2                   AOUT2 ──→ Motor 1
GP3 ───────────────→ PWMB
3V3 ───────────────→ BIN1                   BOUT1 ──→ Motor 2
GND ───────────────→ BIN2                   BOUT2 ──→ Motor 2
3V3 ───────────────→ STBY
```

## Building

Requires [Pico SDK](https://github.com/raspberrypi/pico-sdk) and cmake. The SDK will be fetched automatically during build.

```bash
brew install cmake    # macOS
```

### Make Commands

| Command | Description |
|---------|-------------|
| `make` | Build firmware (.uf2) |
| `make clean` | Clean firmware build |
| `make flash` | Build and flash to device |
| `make sim` | Build SDL simulator |
| `make sim-run` | Run simulator |
| `make sim-clean` | Clean simulator build |
| `make help` | Show all commands |

## Simulator

SDL-based simulator for fast UI iteration without flashing hardware.

```bash
brew install sdl2     # install SDL2

make sim              # build simulator
make sim-run          # run simulator
make sim-clean        # clean simulator build
```

### Simulator Controls

| Key | Action |
|-----|--------|
| ←/↑ | Previous item (CCW) |
| →/↓ | Next item (CW) |
| Enter/Space | Select / Start |
| ESC | Quit |

### Simulator Structure

```
sim/
├── sim_main.c      # Main loop with SDL events
├── sim_display.c   # ST7735 emulation via SDL (3x scale)
├── sim_encoder.c   # Keyboard → encoder events
├── sim_motor.c     # Motor stubs (printf output)
├── sim_pico.h      # Pico SDK stubs
└── pico/stdlib.h   # Fake header for compatibility
```

## Flashing

**RP2040-Zero:**
1. Hold **BOOT** button
2. Connect USB-C cable (or tap **RESET** while holding **BOOT**)
3. Run `make flash` or copy `build/card_shuffler.uf2` to `RPI-RP2` drive
4. Device will reboot and run the firmware

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

```
.------..------..------..------..------..------.
|A.--. ||D.--. ||W.--. ||0.--. ||R.--. ||D.--. |
| (\/) || :/\: || :/\: || :/\: || :(): || :/\: |
| :\/: || (__) || :\/: || :\/: || ()() || (__) |
| '--'A|| '--'D|| '--'W|| '--'0|| '--'R|| '--'D|
`------'`------'`------'`------'`------'`------'
```
