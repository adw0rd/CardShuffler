# Session Context

## What we did
1. Explored the CardShuffler project structure and architecture
2. Discussed TB6612FNG motor driver usage (PWM control on GPIO 2, 3)
3. Created wiring diagram: `docs/wiring-diagram.txt`
4. Committed the wiring diagram
5. Updated ST7735 pin names in wiring diagram to match display board labels (SCL, RES)
6. Added `MOTOR_MAX_SPEED 65` in `config.h` to limit PWM for FA-130 motors
7. Applied max speed limit in `motor_set_speed()`, `motor_ramp_to()`, `motor_ramp_both()`
8. Added bootloader reboot on long press (`reset_usb_boot`)
9. Saved all component photos locally in `docs/`, updated README to use local paths
10. Added TB6612FNG, LX-LCBST, LiPo, FA-130 motor photos
11. Updated wiring diagram: TB6612FNG powered directly from LX-LCBST 5V
12. Fixed README: pin assignments (Encoder BTN=GP4, Extra=GP16, A=GP15, B=GP14)
13. Fixed README: renamed "Custom" strategy to "Random"
14. Added missing components to Hardware table (FA-130, LX-LCBST, Li-Po)

## Pending changes
None — all changes committed and pushed.

## Hardware
- **RP2040-Zero** — microcontroller
- **ST7735** 128x160 — SPI display (3V3, GPIO 5-10)
- **KY-040** — rotary encoder (GPIO 4, 14, 15, internal pull-ups)
- **TB6612FNG** — motor driver (GPIO 2-3 PWM, VCC+VM from LX-LCBST 5V)
- **2x FA-130** — DC motors ∅20mm, 1.5–3V, PWM limited to 65% (~3.25V)
- **LX-LCBST** — boost converter 3.7V → 5V
- **1S Li-Po** — 3.7V 500mAh+ battery

## Photos in docs/
- `card-shuffler.png` — шафл-машинка
- `st7735.png` — дисплей
- `rp2040-zero.jpg` — микроконтроллер
- `ky040.png` — энкодер
- `tb6612fng.jpg` — драйвер моторов
- `motor-fa130.jpg` — мотор
- `lx-lcbst.jpg` — boost converter
- `lipo.jpg` — аккумулятор
- `display-menu.gif` — меню на дисплее

## Project summary
Card Shuffler firmware for RP2040-Zero:
- 6 shuffle strategies: Quick, Riffle, Strip, Wash, Box, Random
- State machine: MENU → SHUFFLING → STOPPING
- PWM motor control (25kHz) with soft start/stop ramping

## Build
```bash
make        # build
make flash  # build and flash
```
