#pragma once
#include "includes/glm/glm.hpp"
#include "src/gameManager.hpp"

struct Rigidbody
{
    glm::vec3 deltaPosition;

    Rigidbody(glm::vec3 position, glm::vec3 direction)
    {
        this->deltaPosition = position + (direction * GameManager::deltaTime);
    }
};