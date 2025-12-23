#include <pbkit/pbkit.h>
#include <hal/debug.h>
#include <hal/video.h>
#include "input/controller.h"
#include "definitions.h"
#include "perfmetrics.h"
#include "viewport.h"


struct Viewport viewport;

void CleanupViewport()
{
    SDL_VideoQuit();
    pb_show_debug_screen();
    pb_kill();
}

void RenderViewport()
{
    // Wait for a VBLANK signal, then reset pbkit and clear the screen with the viewport's clear color
    pb_wait_for_vbl();
    pb_reset();
    pb_target_back_buffer();
    pb_erase_text_screen();
    pb_fill(0, 0, viewport.width, viewport.height, viewport.clearColor);

    // Wait for the start-of-frame steps to finish
    while(pb_busy());

    // Draw performance metrics
    pb_print("FPS: %d (%dms, %d frames)\n", fps, frameTime, frameCount);
    pb_print(
        "UPTIME: %02d:%02d:%02d.%d\n",
        gameUptime / TICKS_TO_HOURS,
        (gameUptime / TICKS_TO_MINUTES) % 60,
             (gameUptime / TICKS_TO_SECONDS) % 60,
             gameUptime % TICKS_TO_SECONDS
    );

    pb_print("CONTROLLERS: %d\n", controllerState.controllerCount);
    pb_draw_text_screen();

    // Wait for text drawing to finish
    while(pb_busy());

    // Update the SDL2 window
    SDL_UpdateWindowSurface(viewport.SDLWindow);

    // Swap the front and back buffers when possible
    while (pb_finished());
}

int InitViewport(int width, int height, int colorDepth, uint32_t clearColor)
{
    // Configure the framebuffers and initialize pbkit
    debugPrint("[INFO] Initializing viewport (%dx%d@%d)...\n", width, height, colorDepth);
    XVideoSetMode(width, height, colorDepth, REFRESH_DEFAULT);

    debugPrint("[INFO] Initializing pbkit...\n");
    int initStatus = pb_init();
    if (initStatus != 0)
    {
        debugPrint("pb_init init failed (error %d)\n", initStatus);
        return initStatus;
    }

    debugPrint("[INFO] Initializing SDL2...\n");
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    int SDLVideoInitStatus = SDL_VideoInit(NULL);

    if (SDLVideoInitStatus < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL2 video.\n");
        debugPrint("SDL2 init error: %s\n", SDL_GetError());
        return SDLVideoInitStatus;
    }

    // Create the SDL window and make sure it exists
    debugPrint("[INFO] Creating SDL2 window (%dx%d)...\n", viewport.width, viewport.height);
    viewport.SDLWindow = SDL_CreateWindow(
        "Arbiter Engine",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        viewport.width,
        viewport.height,
        SDL_WINDOW_SHOWN
    );

    if(viewport.SDLWindow == NULL)
    {
        SDL_VideoQuit();
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create SDL2 window.\n");
        debugPrint("SDL2 window init error: %s\n", SDL_GetError());
        return 2;
    }

    // Set the viewport's dimensions, clear color, and color depth properties
    pb_show_front_screen();
    viewport.clearColor = clearColor;
    viewport.colorDepth = colorDepth;
    viewport.height = pb_back_buffer_height();
    viewport.width = pb_back_buffer_width();

    return 0;
}
