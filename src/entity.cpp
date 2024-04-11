#include "src/entity.hpp"
#include "includes/glad/glad.h"
#include "src/gameManager.hpp"
#include "entity.hpp"
#include "includes/sole/sole.hpp"

Entity::Entity(const std::shared_ptr<SpriteSheet> spriteSheet)
{
    this->uuid = sole::uuid4().str();
    this->spriteSheet = spriteSheet;
    this->modelLocation = glGetUniformLocation(GameManager::shaderProgram, "model");
}

void Entity::draw()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    glUniformMatrix4fv(this->modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, this->spriteIndex, 6);
}

void Entity::setSpriteIndex(const std::string spriteName)
{
    this->spriteIndex = this->spriteSheet->spriteIndexes[spriteName];
}