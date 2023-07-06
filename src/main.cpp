// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

// SDL
#include "includes/SDL2/SDL.h"

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
    SDL_GL_SetSwapInterval(1);

    window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 685, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC);
    /* SDL_WarpMouseInWindow(window, 685 / 2, 600 / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);            */

    openGLContext = SDL_GL_CreateContext(window);
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
    glUseProgram(GameManager::shaderProgram);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    GameManager::camera = std::make_unique<Camera>();
    GameManager::inputSystem = std::make_unique<InputSystem>();
    GameManager::physicsEngine = std::make_unique<PhysicsEngine>();

    // Mario Sprite Sheet
    {
        std::shared_ptr<SpriteSheet> marioSpriteSheet = std::make_shared<SpriteSheetMario>("assets/sprites/mario.png", 16.0f, 32.0f, 1.0f, 2.0f);
        GameManager::marioSpriteSheet = marioSpriteSheet;
        GameManager::marioSpriteSheet->use();

        std::shared_ptr<Entity> entityMario = std::make_shared<Entity>(GameManager::marioSpriteSheet);
        GameManager::dynamicEntities[entityMario->uuid] = entityMario;
        GameManager::playerEntity = entityMario;
    }

    // Blocks Sprite Sheet
    {
        std::shared_ptr<SpriteSheet> blocksSpriteSheet = std::make_shared<SpriteSheetBlocks>("assets/sprites/blocks.png", 16.0f, 16.0f, 1.0f, 1.0f);
        GameManager::blocksSpriteSheet = blocksSpriteSheet;
        GameManager::blocksSpriteSheet->use();
    }

    // Set Floor Entities
    std::vector<std::shared_ptr<Entity>> entities;

    // Platform Right
    for (int i = 3; i < 7; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = i;
        entityBlock->position.y = -3.0f;

        GameManager::staticEntities[entityBlock->uuid] = entityBlock;
        entities.push_back(entityBlock);
    }

    // Platform Left
    for (int i = -7; i < -4; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = i;
        entityBlock->position.y = -2.0f;

        GameManager::staticEntities[entityBlock->uuid] = entityBlock;
        entities.push_back(entityBlock);
    }

    // Floor
    for (int i = -8; i < 8; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = i;
        entityBlock->position.y = -7.0f;

        GameManager::staticEntities[entityBlock->uuid] = entityBlock;
        entities.push_back(entityBlock);
    }

    // Wall right
    for (int i = -6; i < 6; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = 7.0f;
        entityBlock->position.y = i;

        GameManager::staticEntities[entityBlock->uuid] = entityBlock;
        entities.push_back(entityBlock);
    }

    // Wall left
    for (int i = -6; i < 6; i++)
    {
        std::shared_ptr<Entity> entityBlock = std::make_shared<Entity>(GameManager::blocksSpriteSheet);
        entityBlock->position.x = -8.0f;
        entityBlock->position.y = i;

        GameManager::staticEntities[entityBlock->uuid] = entityBlock;
        entities.push_back(entityBlock);
    }

    float lastTick = SDL_GetTicks();
    float milli = 0.0f;
    float lastTime = 0.0f;
    float nFrames = 0.0f;
    /* float deltaTime = 0.0f; */
    while (GameManager::isGameRunning)
    {
        milli = SDL_GetTicks() - lastTick;
        lastTick = SDL_GetTicks();
        GameManager::deltaTime = milli * 0.001f;

        { // Event Handling BLOCK
            GameManager::inputSystem->update();
        }

        { // Physics Engine Block
            GameManager::physicsEngine->update();
        }

        { // RENDER BLOCK
            glUseProgram(GameManager::shaderProgram);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // ---

            GameManager::marioSpriteSheet->use();
            GameManager::playerEntity->setSpriteIndex("idle-right-small");
            GameManager::playerEntity->draw();

            GameManager::blocksSpriteSheet->use();
            for (auto &&e : entities)
                e->draw();

            // ---
            SDL_GL_SwapWindow(window); // Render
        }

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

    glDeleteProgram(GameManager::shaderProgram);

    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}