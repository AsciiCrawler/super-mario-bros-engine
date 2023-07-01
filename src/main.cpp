#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#include "includes/SDL2/SDL.h"
#include "includes/glad/glad.h"
#include "includes/stb_image.h"

#include "string"
#include "memory"
#include <typeinfo>
#include <typeindex>

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"

#include "iostream"

SDL_Window *window;
SDL_GLContext openGLContext;

void createWindow()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC);
    SDL_WarpMouseInWindow(window, 800 / 2, 600 / 2); // FPS Camera
    SDL_SetRelativeMouseMode(SDL_TRUE);              // FPS Camera

    openGLContext = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }
}

int main(int ArgCount, char **Args)
{
    createWindow();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    SDL_GL_SetSwapInterval(1);

    float lastTick = SDL_GetTicks();
    float milli = 0.0f;
    float lastTime = 0.0f;
    float nFrames = 0.0f;
    float deltaTime = 0.0f;

    bool isGameRunning = true;
    while (isGameRunning)
    {
        milli = SDL_GetTicks() - lastTick;
        lastTick = SDL_GetTicks();
        deltaTime = milli * 0.001f;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isGameRunning = false;
                    break;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(window);

        ++nFrames;
        if (SDL_GetTicks() > lastTime)
        {
            float ms = (nFrames > 0 ? 1000.0f / nFrames : 0.0f);
            std::string performanceInfoString;

            performanceInfoString += "FPS: " + std::to_string(int(ms > 0 ? 1000 / ms : 0)) + "\tms: " + std::to_string(ms);
            const char *performanceInfoChars = performanceInfoString.c_str();

            SDL_SetWindowTitle(window, ms > 0 ? performanceInfoChars : "fps = 0 ms = 0");

            nFrames = 0;
            lastTime += 1000;
        }
    }

    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}