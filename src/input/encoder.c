#include "encoder.h"
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LONG_PRESS_MS 1000

static volatile int8_t encoder_delta = 0;
static volatile bool button_pressed = false;
static volatile bool button_event = false;
static uint32_t button_press_time = 0;
static bool long_press_fired = false;

static uint8_t last_state = 0;

// Encoder state machine lookup table
// Index: (last_state << 2) | current_state
// Values: 0 = no change, 1 = CW, -1 = CCW
static const int8_t encoder_table[] = {
    0,  1, -1,  0,
   -1,  0,  0,  1,
    1,  0,  0, -1,
    0, -1,  1,  0
};

static void encoder_callback(uint gpio, uint32_t events) {
    (void)events;

    if (gpio == PIN_ENC_A || gpio == PIN_ENC_B) {
        uint8_t a = gpio_get(PIN_ENC_A);
        uint8_t b = gpio_get(PIN_ENC_B);
        uint8_t current_state = (a << 1) | b;

        int8_t delta = encoder_table[(last_state << 2) | current_state];
        encoder_delta += delta;
        last_state = current_state;
    }
}

static void button_callback(uint gpio, uint32_t events) {
    (void)gpio;

    static uint32_t last_interrupt_time = 0;
    uint32_t now = to_ms_since_boot(get_absolute_time());

    // Debounce
    if (now - last_interrupt_time < ENCODER_DEBOUNCE_MS) {
        return;
    }
    last_interrupt_time = now;

    if (events & GPIO_IRQ_EDGE_FALL) {
        button_pressed = true;
        button_event = true;
        button_press_time = now;
        long_press_fired = false;
    }
    if (events & GPIO_IRQ_EDGE_RISE) {
        button_pressed = false;
        if (!long_press_fired) {
            button_event = true;
        }
    }
}

void encoder_init(void) {
    // Initialize encoder pins
    gpio_init(PIN_ENC_A);
    gpio_init(PIN_ENC_B);
    gpio_set_dir(PIN_ENC_A, GPIO_IN);
    gpio_set_dir(PIN_ENC_B, GPIO_IN);
    gpio_pull_up(PIN_ENC_A);
    gpio_pull_up(PIN_ENC_B);

    // Initialize button pin
    gpio_init(PIN_ENC_BTN);
    gpio_set_dir(PIN_ENC_BTN, GPIO_IN);
    gpio_pull_up(PIN_ENC_BTN);

    // Read initial state
    last_state = (gpio_get(PIN_ENC_A) << 1) | gpio_get(PIN_ENC_B);

    // Set up interrupts for encoder
    gpio_set_irq_enabled_with_callback(PIN_ENC_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_callback);
    gpio_set_irq_enabled(PIN_ENC_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    // Set up interrupts for button
    gpio_set_irq_enabled_with_callback(PIN_ENC_BTN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &button_callback);
}

encoder_event_t encoder_poll(void) {
    // Check for rotation
    if (encoder_delta >= 4) {
        encoder_delta -= 4;
        return ENC_EVENT_CW;
    }
    if (encoder_delta <= -4) {
        encoder_delta += 4;
        return ENC_EVENT_CCW;
    }

    // Check for long press
    if (button_pressed && !long_press_fired) {
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if (now - button_press_time > LONG_PRESS_MS) {
            long_press_fired = true;
            return ENC_EVENT_LONG_PRESS;
        }
    }

    // Check for button event
    if (button_event) {
        button_event = false;
        if (button_pressed) {
            return ENC_EVENT_PRESS;
        } else if (!long_press_fired) {
            return ENC_EVENT_RELEASE;
        }
    }

    return ENC_EVENT_NONE;
}

bool encoder_button_held(void) {
    return button_pressed;
}
