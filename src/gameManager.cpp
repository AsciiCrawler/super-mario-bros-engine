#include "src/gameManager.hpp"
bool GameManager::isGameRunning = true;

unsigned int GameManager::vertexShader;
unsigned int GameManager::fragmentShader;
unsigned int GameManager::shaderProgram;

float GameManager::deltaTime = 0.0f;

std::unique_ptr<Camera> GameManager::camera;
std::unique_ptr<InputSystem> GameManager::inputSystem;

std::unique_ptr<Entity> GameManager::playerEntity;

std::shared_ptr<SpriteSheet> GameManager::marioSpriteSheet;
std::shared_ptr<SpriteSheet> GameManager::blocksSpriteSheet;