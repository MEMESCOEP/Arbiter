#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <SDL.h>

#define DEFAULT_VIEWPORT_CLEAR_COLOR 0xFF161616
#define DEFAULT_VIEWPORT_COLOR_DEPTH 32
#define DEFAULT_VIEWPORT_HEIGHT      480
#define DEFAULT_VIEWPORT_WIDTH       640

struct Viewport {
    SDL_Window* SDLWindow;
    uint32_t clearColor;
    int colorDepth;
    int height;
    int width;
};

extern struct Viewport currentViewport;

void CleanupViewport();
void RenderViewport();
int InitViewport(int width, int height, int colorDepth, uint32_t clearColor);

#endif