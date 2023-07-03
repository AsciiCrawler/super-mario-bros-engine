#include "src/gameManager.hpp"
bool GameManager::isGameRunning = true;
GLuint GameManager::vertexShader;
GLuint GameManager::fragmentShader;
GLuint GameManager::shaderProgram;
std::unique_ptr<Camera> GameManager::camera;