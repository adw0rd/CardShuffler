// SDL-based ST7735 display emulator
#include "display/st7735.h"
#include "config.h"
#include <SDL.h>
#include <string.h>

#define SCALE 3

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static uint16_t framebuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
static uint16_t disp_width = DISPLAY_WIDTH;
static uint16_t disp_height = DISPLAY_HEIGHT;
static uint8_t rotation = 0;
static bool dirty = true;

// Convert RGB565 to RGB888
static void rgb565_to_rgb888(uint16_t color, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = ((color >> 11) & 0x1F) << 3;
    *g = ((color >> 5) & 0x3F) << 2;
    *b = (color & 0x1F) << 3;
}

void st7735_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL init failed: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(
        "Card Shuffler Simulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH, DISPLAY_HEIGHT
    );

    memset(framebuffer, 0, sizeof(framebuffer));
    dirty = true;
}

void st7735_set_rotation(uint8_t rot) {
    rotation = rot & 3;
    if (rotation & 1) {
        disp_width = DISPLAY_HEIGHT;
        disp_height = DISPLAY_WIDTH;
    } else {
        disp_width = DISPLAY_WIDTH;
        disp_height = DISPLAY_HEIGHT;
    }

    // Resize window
    SDL_SetWindowSize(window, disp_width * SCALE, disp_height * SCALE);

    // Recreate texture with new dimensions
    if (texture) SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        disp_width, disp_height
    );
}

void st7735_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // Not needed for simulator - we use direct pixel access
    (void)x0; (void)y0; (void)x1; (void)y1;
}

void st7735_fill_screen(uint16_t color) {
    for (int i = 0; i < disp_width * disp_height; i++) {
        framebuffer[i] = color;
    }
    dirty = true;
}

void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= disp_width || y >= disp_height) return;
    framebuffer[y * disp_width + x] = color;
    dirty = true;
}

void st7735_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t j = y; j < y + h && j < disp_height; j++) {
        for (uint16_t i = x; i < x + w && i < disp_width; i++) {
            framebuffer[j * disp_width + i] = color;
        }
    }
    dirty = true;
}

void st7735_write_data(const uint8_t *data, size_t len) {
    (void)data; (void)len;
}

void st7735_write_data16(const uint16_t *data, size_t len) {
    (void)data; (void)len;
}

void st7735_backlight(bool on) {
    (void)on;
}

uint16_t st7735_get_width(void) {
    return disp_width;
}

uint16_t st7735_get_height(void) {
    return disp_height;
}

// Simulator-specific functions
void sim_display_update(void) {
    if (!dirty) return;

    uint8_t *pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

    for (int y = 0; y < disp_height; y++) {
        for (int x = 0; x < disp_width; x++) {
            uint16_t color = framebuffer[y * disp_width + x];
            uint8_t r, g, b;
            rgb565_to_rgb888(color, &r, &g, &b);
            int idx = y * pitch + x * 3;
            pixels[idx + 0] = r;
            pixels[idx + 1] = g;
            pixels[idx + 2] = b;
        }
    }

    SDL_UnlockTexture(texture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    dirty = false;
}

void sim_display_cleanup(void) {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

// Called from strategy_sleep_ms to force display update
void st7735_flush(void) {
    sim_display_update();
}
