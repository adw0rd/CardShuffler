#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ENC_EVENT_NONE,
    ENC_EVENT_CW,       // Clockwise rotation
    ENC_EVENT_CCW,      // Counter-clockwise rotation
    ENC_EVENT_PRESS,    // Button pressed
    ENC_EVENT_RELEASE,  // Button released
    ENC_EVENT_LONG_PRESS // Long press (>1s)
} encoder_event_t;

void encoder_init(void);
encoder_event_t encoder_poll(void);
bool encoder_button_held(void);

#endif // ENCODER_H
