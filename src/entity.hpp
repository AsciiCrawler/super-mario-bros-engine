#pragma once
#include "includes/glm/glm.hpp"
#include "src/spriteSheet/spriteSheet.hpp"
#include "memory"
#include "string"

class Entity
{
public:
    Entity(const std::shared_ptr<SpriteSheet> spriteSheet);

    std::string uuid;
    std::shared_ptr<SpriteSheet> spriteSheet;
    unsigned int modelLocation = 0;

    void draw();
    void setSpriteIndex(const std::string spriteName);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -5.0f);
    glm::vec3 physicsDirection = glm::vec3(0.0f, 0.0f, 0.0f);
    int spriteIndex = 0;
};