#include <windows.h>
#include "input/controller.h"
#include "definitions.h"
#include "perfmetrics.h"
#include "viewport.h"

int waitTime = 0;
int FPS_MS = 1;

void SetTargetFPS(int targetFPS)
{
    FPS_MS = (int)(1000.0f / (float)targetFPS);
}

int main(void)
{
    debugPrint("[== ARBITER ENGINE ==]\n");

    // Keep track of when the engine started
    engineStart = GetTickCount();
    debugPrint("[INFO] Engine started at %d.\n", engineStart);

    // Configure the viewport
    int viewportInitStatus = InitViewport(DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT, DEFAULT_VIEWPORT_COLOR_DEPTH, DEFAULT_VIEWPORT_CLEAR_COLOR);
    if (viewportInitStatus != 0)
    {
        debugPrint("[ERROR] Viewport initialization failed with error code %d!\n", viewportInitStatus);
        while(1);
    }

    // Initialize the controllers
    int SDLControllerStatus = InitControllers();
    if (SDLControllerStatus != 0)
    {
        debugPrint("[ERROR] SDL controller initialization failed with error code %d!\n", SDLControllerStatus);
        while(1);
    }

    // Initialize XAudio
    
    // Set the FPS target
    // NOTE: This target may not be reached in all situations. User scene and code complexity and optimization may affect the maximum performance 
    debugPrint("[INFO] Setting target FPS to %d...\n", DEFAULT_FPS);
    SetTargetFPS(DEFAULT_FPS);

    // Keep track of when the game loop started
    gameStart = GetTickCount();
    last = gameStart;
    now = gameStart;

    debugPrint("[INFO] Init finished.\n");

    while(1)
    {
        // Keep track of the game's uptime and frame start time
        gameUptime = now - gameStart;
        frameStart = GetTickCount();
        
        // Get player input
        UpdateControllerStates();

        // Render 3D and 2D graphics
        RenderViewport();

        // Keep track of performance metrics
        frameCount++;
        frames++;
        now = GetTickCount();

        if (now - last > 1000)
        {
            last = now;
            fps = frames;
            frames = 0;
        }

        // Wait for the target frame time minus the time this frame took in order to reach the target framerate
        frameEnd = GetTickCount();
        waitTime = FPS_MS - (frameEnd - frameStart);

        if (waitTime > 0)
        {
            Sleep(waitTime);
        }

        // It's important that we get the frame time after all operations in this loop iteration have completed. This way, we get the most accurate measurement
        frameTime = GetTickCount() - frameStart;
    }

    // Clean up
    // NOTE: This should not be reachable normally, it's here as a just-in-case/formality thing
    CleanupViewport();
    return 0;
}
