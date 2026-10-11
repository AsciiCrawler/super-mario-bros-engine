// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

// SDL
#include <SDL3/SDL.h>

#include <algorithm>
#include <cstdio>

#include "src/utilities.hpp"
#include "src/gameManager.hpp"
#include "src/openGLToolkit.hpp"
#include "src/spriteSheet/spriteSheet.hpp"
#include "src/spriteSheet/spriteSheetMario.hpp"
#include "src/spriteSheet/spriteSheetBlocks.hpp"
#include "src/camera.hpp"
#include "src/entity.hpp"
#include "src/inputSystem/inputSystem.hpp"
#include "src/physicsEngine/physicsEngine.hpp"
#include "src/spriteBatch.hpp"

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

    window = SDL_CreateWindow("OpenGL Test", 685, 600, SDL_WINDOW_OPENGL);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    /* SDL_WarpMouseInWindow(window, 685 / 2, 600 / 2);
    SDL_SetWindowRelativeMouseMode(window, true);            */

    openGLContext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }
}

// 3 Position - 2 Texture position
float vertices[] = {
    1.0f, 1.0f, 1.0f, 1.0f, // top right
    1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    0.0f, 0.0f, 0.0f, 0.0f, // bottom left
    0.0f, 1.0f, 0.0f, 1.0f  // top left
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main(int ArgCount, char **Args)
{
    createWindow();
    OpenGLToolkit::LoadVertexShader();
    OpenGLToolkit::LoadFragmentShader();
    OpenGLToolkit::LoadShaderProgram();
    glUseProgram(GameManager::shaderProgram); // The only shader program, bound for the whole run
    glClearColor(0.4823529411764706f, 0.4627450980392157f, 0.9686274509803922f, 1.0f);

    GameManager::camera = std::make_unique<Camera>();
    GameManager::inputSystem = std::make_unique<InputSystem>();
    GameManager::physicsEngine = std::make_unique<PhysicsEngine>();

    // Mario Sprite Sheet
    {
        std::shared_ptr<SpriteSheet> marioSpriteSheet = std::make_shared<SpriteSheetMario>("assets/sprites/mario.png", 16.0f, 32.0f, 1.0f, 2.0f);
        GameManager::marioSpriteSheet = marioSpriteSheet;
        GameManager::marioSpriteSheet->use();

        std::shared_ptr<Entity> entityMario = std::make_shared<Entity>(GameManager::marioSpriteSheet);
        GameManager::dynamicEntities.push_back(entityMario);
        GameManager::playerEntity = entityMario;
        GameManager::playerEntity->setSpriteIndex("idle-right-small");
    }

    // Blocks Sprite Sheet
    {
        std::shared_ptr<SpriteSheet> blocksSpriteSheet = std::make_shared<SpriteSheetBlocks>("assets/sprites/blocks.png", 16.0f, 16.0f, 1.0f, 1.0f);
        GameManager::blocksSpriteSheet = blocksSpriteSheet;
        GameManager::blocksSpriteSheet->use();
    }

    // Set Floor Entities
    // Platform Right
    for (int i = 3; i < 7; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = i;
        entityBlock->position.y = -3.0f;

        GameManager::staticEntities.push_back(entityBlock);
    }

    // Platform Left
    for (int i = -7; i < -4; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = i;
        entityBlock->position.y = -2.0f;

        GameManager::staticEntities.push_back(entityBlock);
    }

    // Floor
    for (int i = -8; i < 8; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = i;
        entityBlock->position.y = -7.0f;

        GameManager::staticEntities.push_back(entityBlock);
    }

    // Wall right
    for (int i = -6; i < 6; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = 7.0f;
        entityBlock->position.y = i;

        GameManager::staticEntities.push_back(entityBlock);
    }

    // Wall left
    for (int i = -6; i < 6; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = -8.0f;
        entityBlock->position.y = i;

        GameManager::staticEntities.push_back(entityBlock);
    }

    GameManager::tileMap.build(GameManager::staticEntities);

    // One batch per sprite sheet: static tiles are uploaded once, moving sprites every frame
    std::vector<std::unique_ptr<SpriteBatch>> staticBatches;
    std::vector<std::unique_ptr<SpriteBatch>> dynamicBatches;
    auto batchFor = [](std::vector<std::unique_ptr<SpriteBatch>> &batches, const std::shared_ptr<SpriteSheet> &spriteSheet, const unsigned int usage) -> SpriteBatch &
    {
        for (auto &&batch : batches)
            if (batch->spriteSheet == spriteSheet)
                return *batch;
        batches.push_back(std::make_unique<SpriteBatch>(spriteSheet, usage));
        return *batches.back();
    };

    for (auto &&e : GameManager::staticEntities)
        batchFor(staticBatches, e->spriteSheet, GL_STATIC_DRAW).add(e->spriteIndex, e->position);
    for (auto &&batch : staticBatches)
        batch->upload();

    for (auto &&e : GameManager::dynamicEntities)
        e->previousPosition = e->position;

    const double fixedStep = 1.0 / 120.0; // Simulation runs at a fixed rate, independent of FPS
    const double maxFrameTime = 0.25;     // Caps catch-up after a stall
    const double counterFrequency = (double)SDL_GetPerformanceFrequency();
    Uint64 lastCounter = SDL_GetPerformanceCounter();
    Uint64 fpsCounterStart = lastCounter;
    double accumulator = 0.0;
    int nFrames = 0;
    while (GameManager::isGameRunning)
    {
        const Uint64 counter = SDL_GetPerformanceCounter();
        accumulator += std::min((counter - lastCounter) / counterFrequency, maxFrameTime);
        lastCounter = counter;

        { // Event Handling BLOCK
            GameManager::inputSystem->update();
        }

        { // Simulation BLOCK
            while (accumulator >= fixedStep)
            {
                for (auto &&e : GameManager::dynamicEntities)
                    e->previousPosition = e->position;

                GameManager::inputSystem->applyToPlayer((float)fixedStep);
                GameManager::physicsEngine->update((float)fixedStep);

                GameManager::simulationMilliseconds += fixedStep * 1000.0;
                accumulator -= fixedStep;
            }
        }

        { // RENDER BLOCK
            // Fraction of a step not yet simulated, used to blend the last two simulated positions
            const float alpha = (float)(accumulator / fixedStep);

            glClear(GL_COLOR_BUFFER_BIT);
            // ---

            for (auto &&batch : dynamicBatches)
                batch->clear();
            for (auto &&e : GameManager::dynamicEntities)
                batchFor(dynamicBatches, e->spriteSheet, GL_STREAM_DRAW).add(e->spriteIndex, glm::mix(e->previousPosition, e->position, alpha));
            for (auto &&batch : dynamicBatches)
                batch->upload();

            // No depth test: draw order decides what is on top, so moving sprites go last
            for (auto &&batch : staticBatches)
                batch->draw();
            for (auto &&batch : dynamicBatches)
                batch->draw();

            // ---
            SDL_GL_SwapWindow(window); // Render
        }

        ++nFrames;
        const double elapsedSeconds = (counter - fpsCounterStart) / counterFrequency;
        if (elapsedSeconds >= 1.0)
        {
            char performanceInfo[64];
            snprintf(performanceInfo, sizeof(performanceInfo), "FPS: %d\tms: %f", (int)(nFrames / elapsedSeconds), elapsedSeconds * 1000.0 / nFrames);
            SDL_SetWindowTitle(window, performanceInfo);

            nFrames = 0;
            fpsCounterStart = counter;
        }
    }

    staticBatches.clear(); // GL objects must be released while the context still exists
    dynamicBatches.clear();
    glDeleteProgram(GameManager::shaderProgram);

    SDL_GL_DestroyContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}