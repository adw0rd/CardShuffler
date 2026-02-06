# Session Context

## What we did
1. Explored the CardShuffler project structure and architecture
2. Discussed TB6612FNG motor driver usage (PWM control on GPIO 2, 3)
3. Created wiring diagram: `docs/wiring-diagram.txt`
4. Committed the wiring diagram
5. Updated ST7735 pin names in wiring diagram to match display board labels (SCL, RES)
6. Added `MOTOR_MAX_SPEED 65` in `config.h` to limit PWM for FA-130 motors
7. Applied max speed limit in `motor_set_speed()`, `motor_ramp_to()`, `motor_ramp_both()`

## Pending changes
- `src/main.c` — uncommitted: `reset_usb_boot(0, 0)` on long press for bootloader reboot
- `src/config.h` — uncommitted: added `MOTOR_MAX_SPEED 65`
- `src/motor/motor.c` — uncommitted: speed clamped to `MOTOR_MAX_SPEED`

## Motors
- 2x FA-130 DC motors, ∅20mm
- Nominal voltage: 1.5–3V, max ~4.5V
- Powered via TB6612FNG, VM=5V, PWM limited to 65% (~3.25V effective)
- Photo: `docs/motor-fa130.jpg`

## Project summary
Card Shuffler firmware for RP2040-Zero:
- ST7735 display (128x160, SPI)
- Rotary encoder for navigation
- TB6612FNG motor driver (2 motors, PWM 25kHz)
- 6 shuffle strategies: Quick, Riffle, Strip, Wash, Box, Random
- State machine: MENU → SHUFFLING → STOPPING

## Build
```bash
make        # build
make flash  # build and flash
```
