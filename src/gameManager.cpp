#include "src/gameManager.hpp"
bool GameManager::isGameRunning = true;

unsigned int GameManager::vertexShader;
unsigned int GameManager::fragmentShader;
unsigned int GameManager::shaderProgram;

double GameManager::simulationMilliseconds = 0.0;

std::unique_ptr<Camera> GameManager::camera;
std::unique_ptr<InputSystem> GameManager::inputSystem;
std::unique_ptr<PhysicsEngine> GameManager::physicsEngine;

std::shared_ptr<Entity> GameManager::playerEntity;
std::vector<std::shared_ptr<Entity>> GameManager::dynamicEntities;
std::vector<std::shared_ptr<Entity>> GameManager::staticEntities;
TileMap GameManager::tileMap;

std::shared_ptr<SpriteSheet> GameManager::marioSpriteSheet;
std::shared_ptr<SpriteSheet> GameManager::blocksSpriteSheet;