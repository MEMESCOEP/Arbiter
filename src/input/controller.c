#include "input/controller.h"

struct ControllerState controllerState;

void UpdateControllerStates()
{
    // Update the previous button state array
    // NOTE: This is done at the beginning of input handling so 'button held' events are delayed by one frame
    for (int controllerIndex = 0; controllerIndex < MAX_CONTROLLERS; controllerIndex++)
    {
        for (int buttonIndex = 0; buttonIndex < SDL_CONTROLLER_BUTTON_MAX; buttonIndex++)
        {
            controllerState.wasButtonDown[controllerIndex][buttonIndex] = controllerState.isButtonDown[controllerIndex][buttonIndex];
        }
    }

    // Handle new input events
    while (SDL_PollEvent(&controllerEvent))
    {
        int controllerIndex = 0;

        switch (controllerEvent.type)
        {
            case SDL_CONTROLLERDEVICEADDED:
                SDL_GameController* newController = SDL_GameControllerOpen(controllerEvent.cdevice.which);
                controllerIndex = GetFirstEmptyControllerSlot();
                if (controllerIndex >= 0 && newController != NULL)
                {
                    controllerState.controllers[controllerIndex] = newController;
                    controllerState.controllerCount++;
                }

                break;

            case SDL_CONTROLLERDEVICEREMOVED:
                controllerIndex = FindControllerIndexByID(controllerEvent.cdevice.which);
                if (controllerIndex >= 0)
                {
                    SDL_GameControllerClose(controllerState.controllers[controllerIndex]);
                    controllerState.controllers[controllerIndex] = NULL;
                    controllerState.controllerCount--;
                }

                break;

            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                controllerIndex = FindControllerIndexByID(controllerEvent.cdevice.which);
                if (controllerIndex >= 0)
                {
                    controllerState.isButtonDown[controllerIndex][controllerEvent.cbutton.button] = (controllerEvent.type == SDL_CONTROLLERBUTTONDOWN);
                }
                
                break;

            default:
                break;
        }

        SDL_GameControllerUpdate();
    }
}

int FindControllerIndexByID(SDL_JoystickID ID)
{
    SDL_GameController* controller = SDL_GameControllerFromInstanceID(ID);
    if (!controller) return -1;
    
    for (int controllerIndex = 0; controllerIndex < MAX_CONTROLLERS; controllerIndex++)
    {
        if (controllerState.controllers[controllerIndex] == controller)
            return controllerIndex;
    }

    return -1;
}

int GetFirstEmptyControllerSlot()
{
    for (int controllerIndex = 0; controllerIndex < MAX_CONTROLLERS; controllerIndex++)
    {
        if (controllerState.controllers[controllerIndex] == NULL)
            return controllerIndex;
    }

    return -1;
}

int InitControllers()
{
    // Set the controllers to NULL to avoid random data and to make controller detection easier
    for (uint8_t controllerIndex = 0; controllerIndex < MAX_CONTROLLERS; controllerIndex++)
    {
        controllerState.controllers[controllerIndex] = NULL;
    }

    // Initialize SDL in gamecontroller mode
    int SDLInitStatus = SDL_Init(SDL_INIT_GAMECONTROLLER);
    if (SDLInitStatus != 0)
    {
        debugPrint("[ERROR] SDL controller init failed: %s\n", SDL_GetError());
        return SDLInitStatus;
    }

    return 0;
}