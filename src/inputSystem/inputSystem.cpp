#include "src/inputSystem/inputSystem.hpp"
#include "src/gameManager.hpp"
#include <SDL3/SDL.h>
#include "cmath"

int InputSystem::actionForKey(const int key)
{
    switch (key)
    {
    case SDLK_A:
        return ActionLeft;
    case SDLK_D:
        return ActionRight;
    case SDLK_LSHIFT:
        return ActionRun;
    case SDLK_W:
        return ActionJump;
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
            if (action == ActionJump && this->actions[ActionJump] == false)
                this->lastJumpMilliseconds = GameManager::simulationMilliseconds;

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

void InputSystem::applyToPlayer(const float step)
{
    const double now = GameManager::simulationMilliseconds;

    if (this->actions[ActionLeft])
        GameManager::playerEntity->velocity.x = this->actions[ActionRun] ? -10.5f : -7.5f;

    if (this->actions[ActionRight])
        GameManager::playerEntity->velocity.x = this->actions[ActionRun] ? 10.5f : 7.5f;

    if (this->actions[ActionJump])
    {
        const int firstPart = 225;
        const int secondPart = 480;
        if (this->lastJumpMilliseconds + firstPart > now)
        {
            this->jumpState = 1;
            if (std::abs(GameManager::playerEntity->velocity.x) > 8.5F)
            {
                GameManager::playerEntity->velocity.y = 15.5f;
            }
            else
            {
                GameManager::playerEntity->velocity.y = 13.0f;
            }
        }

        if (this->lastJumpMilliseconds + firstPart < now && this->lastJumpMilliseconds + secondPart > now)
        {
            this->jumpState = 2;
            GameManager::playerEntity->velocity.y -= 55.0f * step;
            if (GameManager::playerEntity->velocity.y < 0.2f)
            {
                GameManager::playerEntity->velocity.y = 0.2f;
            }
        }

        if (this->lastJumpMilliseconds + secondPart < now)
            this->jumpState = 0;
    }
    else
        this->jumpState = 0;
}

void InputSystem::cancelJump()
{
    this->lastJumpMilliseconds = -1.0e9; // Far enough in the past that every jump window has expired
}
