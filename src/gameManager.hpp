#pragma once
#include "memory"
#include "src/camera.hpp"
#include "src/entity.hpp"
#include "src/tileMap.hpp"
#include "src/spriteSheet/spriteSheet.hpp"
#include "src/inputSystem/inputSystem.hpp"
#include "src/physicsEngine/physicsEngine.hpp"
#include "src/playerController/playerController.hpp"
#include "vector"

class GameManager
{
public:
    static bool isGameRunning; // Default True

    static unsigned int vertexShader;   // Set -> openGLToolkit
    static unsigned int fragmentShader; // Set -> openGLToolkit
    static unsigned int shaderProgram;  // Set -> openGLToolkit

    static double simulationMilliseconds; // Advanced by each fixed simulation step

    static std::unique_ptr<Camera> camera;
    static std::unique_ptr<InputSystem> inputSystem;
    static std::unique_ptr<PhysicsEngine> physicsEngine;
    static std::unique_ptr<PlayerController> playerController;

    static std::shared_ptr<Entity> playerEntity;
    static std::vector<std::shared_ptr<Entity>> dynamicEntities;
    static std::vector<std::shared_ptr<Entity>> staticEntities;
    static TileMap tileMap; // Built from staticEntities

    static std::shared_ptr<SpriteSheet> marioSpriteSheet;
    static std::shared_ptr<SpriteSheet> blocksSpriteSheet;
};
