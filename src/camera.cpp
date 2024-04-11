#include "src/camera.hpp"
#include "src/gameManager.hpp"
#include "includes/glad/glad.h"

Camera::Camera()
{
    const float inverseAspect = 685.0f / 600.0f;
    const float width = (15 * inverseAspect) / 2.0f;
    const float height = 15.0f / 2.0f;

    glm::mat4 projection = glm::ortho(-width, width, -height, height, 0.1f, 100.0f);
    unsigned int projectionLocation = glGetUniformLocation(GameManager::shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    this->viewLocation = glGetUniformLocation(GameManager::shaderProgram, "view");
}

const float inverseAspect = 685.0f / 600.0f;
const float width = (15 * inverseAspect) / 2.0f;
const float height = 15.0f / 2.0f;
void Camera::update()
{
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(this->viewLocation, 1, GL_FALSE, glm::value_ptr(view));
}