#pragma once
#include "memory"
#include "src/camera.hpp"
#include "src/entity.hpp"
#include "src/spriteSheet/spriteSheet.hpp"
#include "src/inputSystem/inputSystem.hpp"

class GameManager
{
public:
    static bool isGameRunning; // Default True
    
    static unsigned int vertexShader;   // Set -> openGLToolkit
    static unsigned int fragmentShader; // Set -> openGLToolkit
    static unsigned int shaderProgram;  // Set -> openGLToolkit

    static float deltaTime;

    static std::unique_ptr<Camera> camera;
    static std::unique_ptr<InputSystem> inputSystem;
    
    static std::unique_ptr<Entity> playerEntity;

    static std::shared_ptr<SpriteSheet> marioSpriteSheet;
    static std::shared_ptr<SpriteSheet> blocksSpriteSheet;
};