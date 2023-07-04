#pragma once
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"

class Camera
{
public:
    Camera();
    void update();
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    unsigned int viewLocation = 0;
};