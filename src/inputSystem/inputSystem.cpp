#include "src/inputSystem/inputSystem.hpp"
#include "src/gameManager.hpp"
#include "includes/SDL2/SDL.h"
#include "iostream"

void InputSystem::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            this->keys[event.key.keysym.sym] = true;

            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                GameManager::isGameRunning = false;
                break;
            }
        }

        if (event.type == SDL_KEYUP)
        {
            this->keys[event.key.keysym.sym] = false;
        }
    }

    for (auto &&e : this->keys)
    {
        if (e.first == SDLK_a && e.second == true)
        {
            GameManager::playerEntity->velocity.x = -5.0f/*  * GameManager::deltaTime */;
        }

        if (e.first == SDLK_d && e.second == true)
        {
            GameManager::playerEntity->velocity.x = 5.0f/*  * GameManager::deltaTime */;
        }

        if (e.first == SDLK_w && e.second == true)
        {
            GameManager::playerEntity->velocity.y = 10.0f/*  * GameManager::deltaTime */;
        }

    }

    /* for (auto &e : this->keys)
    {
        std::cout << "key : " << e.second << std::endl;

        if(e.second == SDLK_a) {
            std::cout << "a is pressed" << std::endl;
        }
    } */
}