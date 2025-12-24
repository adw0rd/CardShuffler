// Simulator main loop
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "config.h"
#include "display/st7735.h"
#include "display/graphics.h"
#include "ui/menu.h"
#include "input/encoder.h"
#include "motor/motor.h"
#include "motor/strategies.h"

// External simulator functions
extern void sim_display_update(void);
extern void sim_display_cleanup(void);
extern void sim_encoder_handle_event(SDL_Event *e);

// Time tracking
static uint32_t start_time = 0;

void sim_sleep_ms(uint32_t ms) {
    SDL_Delay(ms);
}

uint32_t sim_time_us(void) {
    return (SDL_GetTicks() - start_time) * 1000;
}

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
    if (event == ENC_EVENT_PRESS || event == ENC_EVENT_RELEASE) {
        state = STATE_STOPPING;
        strategy_stop();
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    start_time = SDL_GetTicks();

    printf("Card Shuffler Simulator\n");
    printf("Controls:\n");
    printf("  Arrow keys: Navigate menu\n");
    printf("  Enter/Space: Select\n");
    printf("  ESC: Quit\n\n");

    // Initialize hardware emulation
    st7735_init();
    st7735_set_rotation(DISPLAY_ROTATION);
    encoder_init();
    motor_init();

    // Initialize UI
    menu_init();
    menu_draw();

    // Main loop
    bool running = true;
    while (running) {
        // Handle SDL events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            } else {
                sim_encoder_handle_event(&e);
            }
        }

        // Poll encoder (returns events set by sim_encoder_handle_event)
        encoder_event_t event = encoder_poll();

        // State machine
        switch (state) {
            case STATE_MENU:
                handle_menu_input(event);
                break;

            case STATE_SHUFFLING:
                handle_shuffling_input(event);
                menu_draw_progress(strategy_get_progress());

                if (!strategy_is_running()) {
                    state = STATE_MENU;
                    menu_set_shuffling(false);
                    menu_draw_progress(0);
                }
                break;

            case STATE_STOPPING:
                if (!motor_is_running()) {
                    state = STATE_MENU;
                    menu_set_shuffling(false);
                    menu_draw_progress(0);
                }
                break;
        }

        // Update display
        sim_display_update();

        // Frame delay
        SDL_Delay(16);  // ~60 FPS
    }

    sim_display_cleanup();
    return 0;
}
