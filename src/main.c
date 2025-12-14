#include <stdio.h>
#include "pico/stdlib.h"
#include "config.h"
#include "display/st7735.h"
#include "display/graphics.h"
#include "ui/menu.h"
#include "input/encoder.h"
#include "motor/motor.h"
#include "motor/strategies.h"

typedef enum {
    STATE_MENU,
    STATE_SHUFFLING,
    STATE_STOPPING
} app_state_t;

static app_state_t state = STATE_MENU;

static void handle_menu_input(encoder_event_t event) {
    switch (event) {
        case ENC_EVENT_CW:
            menu_next();
            break;

        case ENC_EVENT_CCW:
            menu_prev();
            break;

        case ENC_EVENT_RELEASE:
            // Start shuffling
            state = STATE_SHUFFLING;
            menu_set_shuffling(true);
            strategy_run(menu_get_selected());
            state = STATE_MENU;
            menu_set_shuffling(false);
            menu_draw_progress(0);
            break;

        default:
            break;
    }
}

static void handle_shuffling_input(encoder_event_t event) {
    // Any button press stops shuffling
    if (event == ENC_EVENT_PRESS || event == ENC_EVENT_RELEASE) {
        state = STATE_STOPPING;
        strategy_stop();
    }
}

int main(void) {
    // Initialize stdio for debugging
    stdio_init_all();

    // Initialize hardware
    st7735_init();
    encoder_init();
    motor_init();

    // Initialize UI
    menu_init();
    menu_draw();

    // Main loop
    while (true) {
        encoder_event_t event = encoder_poll();

        switch (state) {
            case STATE_MENU:
                handle_menu_input(event);
                break;

            case STATE_SHUFFLING:
                handle_shuffling_input(event);
                // Update progress bar
                menu_draw_progress(strategy_get_progress());

                // Check if done
                if (!strategy_is_running()) {
                    state = STATE_MENU;
                    menu_set_shuffling(false);
                    menu_draw_progress(0);
                }
                break;

            case STATE_STOPPING:
                // Wait for motors to stop
                if (!motor_is_running()) {
                    state = STATE_MENU;
                    menu_set_shuffling(false);
                    menu_draw_progress(0);
                }
                break;
        }

        sleep_ms(10);
    }

    return 0;
}
