#include "src/inputSystem/inputSystem.hpp"
#include "src/gameManager.hpp"
#include "includes/SDL2/SDL.h"
#include "iostream"

void InputSystem::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        const SDL_Keycode key = event.key.keysym.sym;
        if (event.type == SDL_KEYDOWN)
        {
            if (key == SDLK_ESCAPE)
            {
                GameManager::isGameRunning = false;
                break;
            }

            if (key == SDLK_w && this->keys[key] == false)
            {
                this->lastJumpMilliseconds = SDL_GetTicks();
                std::cout << "LastMillisecond: " << this->lastJumpMilliseconds << std::endl;
            }

            this->keys[key] = true;
        }

        if (event.type == SDL_KEYUP)
            this->keys[event.key.keysym.sym] = false;
    }

    for (auto &&e : this->keys)
    {
        if (e.first == SDLK_a && e.second == true)
        {

            if (this->keys[SDLK_LSHIFT] == true)
            {
                GameManager::playerEntity->velocity.x = -10.5f;
            }
            else
            {
                GameManager::playerEntity->velocity.x = -7.5f;
            }
        }

        if (e.first == SDLK_d && e.second == true)
        {
            if (this->keys[SDLK_LSHIFT] == true)
            {
                GameManager::playerEntity->velocity.x = 10.5f;
            }
            else
            {
                GameManager::playerEntity->velocity.x = 7.5f;
            }
        }

        if (e.first == SDLK_w && e.second == true)
        {
            const int firstPart = 225;
            const int secondPart = 480;
            if (this->lastJumpMilliseconds + firstPart > SDL_GetTicks())
            {
                this->jumpState = 1;
                if (abs(GameManager::playerEntity->velocity.x) > 8.5F)
                {
                    GameManager::playerEntity->velocity.y = 15.5f;
                }
                else
                {
                    GameManager::playerEntity->velocity.y = 13.0f;
                }
            }

            if (this->lastJumpMilliseconds + firstPart < SDL_GetTicks() && this->lastJumpMilliseconds + secondPart > SDL_GetTicks())
            {
                this->jumpState = 2;
                GameManager::playerEntity->velocity.y -= 55.0f * GameManager::deltaTime;
                if (GameManager::playerEntity->velocity.y < 0.2f)
                {
                    GameManager::playerEntity->velocity.y = 0.2f;
                }
            }

            if (this->lastJumpMilliseconds + secondPart < SDL_GetTicks())
                this->jumpState = 0;
        }

        if (e.first == SDLK_w && e.second == false)
            this->jumpState = 0;

        /* if (e.first == SDLK_w && e.second == false)
        {
            this->isJumping = false;
            this->isPostJumping = false;
        } */
    }

    /* for (auto &e : this->keys)
    {
        std::cout << "key : " << e.second << std::endl;

        if(e.second == SDLK_a) {
            std::cout << "a is pressed" << std::endl;
        }
    } */
}