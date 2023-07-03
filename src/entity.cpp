#include "src/entity.hpp"
#include "includes/glad/glad.h"
#include "src/gameManager.hpp"

void Entity::draw()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    unsigned int modelLoc = glGetUniformLocation(GameManager::shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 48, 6);
}