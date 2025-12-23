#ifndef CONTROLLER_H
#define CONTROLLER_H
#define MAX_CONTROLLERS 4

#include <SDL.h>
#include "definitions.h"

struct ControllerState {
    SDL_GameController* controllers[MAX_CONTROLLERS];               // Stores a list of controllers
    bool wasButtonDown[MAX_CONTROLLERS][SDL_CONTROLLER_BUTTON_MAX]; // Stores the previous pressed state for each button of each controller
    bool isButtonDown[MAX_CONTROLLERS][SDL_CONTROLLER_BUTTON_MAX];  // Stores the current pressed state for each button of each controller
    uint8_t controllerCount;                                        // Stores the number of connetced controllers
};

extern struct ControllerState controllerState;
static SDL_Event controllerEvent;

void UpdateControllerStates();
int GetFirstEmptyControllerSlot();
int FindControllerIndexByID(SDL_JoystickID ID);
int InitControllers();

#endif
