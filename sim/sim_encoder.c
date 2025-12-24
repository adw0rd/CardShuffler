// Keyboard-based encoder emulator
#include "input/encoder.h"
#include <SDL.h>

static encoder_event_t pending_event = ENC_EVENT_NONE;
static bool button_held = false;

void encoder_init(void) {
    pending_event = ENC_EVENT_NONE;
    button_held = false;
}

encoder_event_t encoder_poll(void) {
    encoder_event_t evt = pending_event;
    pending_event = ENC_EVENT_NONE;
    return evt;
}

bool encoder_button_held(void) {
    return button_held;
}

// Simulator-specific: called from main loop
void sim_encoder_handle_event(SDL_Event *e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_UP:
                pending_event = ENC_EVENT_CCW;
                break;
            case SDLK_RIGHT:
            case SDLK_DOWN:
                pending_event = ENC_EVENT_CW;
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                pending_event = ENC_EVENT_PRESS;
                button_held = true;
                break;
        }
    } else if (e->type == SDL_KEYUP) {
        switch (e->key.keysym.sym) {
            case SDLK_RETURN:
            case SDLK_SPACE:
                pending_event = ENC_EVENT_RELEASE;
                button_held = false;
                break;
        }
    }
}
