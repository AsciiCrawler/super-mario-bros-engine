#pragma once
#include "includes/glm/glm.hpp"

class Entity
{
public:
    void draw();
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -5.0f);
};