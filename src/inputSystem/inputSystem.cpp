#include "src/inputSystem/inputSystem.hpp"
#include "src/gameManager.hpp"
#include <SDL3/SDL.h>

InputSystem::InputSystem()
{
    for (int i = 0; i < ActionCount; i++)
        this->lastPressMilliseconds[i] = -1.0e9; // Far enough in the past that no press counts as recent
}

int InputSystem::actionForKey(const int key)
{
    switch (key)
    {
    case SDLK_A:
        return ActionLeft;
    case SDLK_D:
        return ActionRight;
    case SDLK_W:
        return ActionUp;
    case SDLK_S:
        return ActionDown;
    case SDLK_LSHIFT:
        return ActionRun;
    case SDLK_SPACE:
        return ActionJump;
    case SDLK_K:
        return ActionDash;
    default:
        return -1;
    }
}

void InputSystem::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            const SDL_Keycode key = event.key.key;
            if (key == SDLK_ESCAPE)
            {
                GameManager::isGameRunning = false;
                break;
            }

            const int action = actionForKey(key);
            if (action >= 0 && this->actions[action] == false) // Key repeat is not a new press
            {
                this->lastPressMilliseconds[action] = GameManager::simulationMilliseconds;
                this->pressConsumed[action] = false;
            }

            if (action >= 0)
                this->actions[action] = true;
        }

        if (event.type == SDL_EVENT_KEY_UP)
        {
            const int action = actionForKey(event.key.key);
            if (action >= 0)
                this->actions[action] = false;
        }
    }
}

bool InputSystem::isHeld(const Action action) const
{
    return this->actions[action];
}

bool InputSystem::pressedWithin(const Action action, const double milliseconds) const
{
    return !this->pressConsumed[action] && GameManager::simulationMilliseconds - this->lastPressMilliseconds[action] <= milliseconds;
}

void InputSystem::consume(const Action action)
{
    this->pressConsumed[action] = true;
}
