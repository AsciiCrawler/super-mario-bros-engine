#pragma once
#include "memory"
#include "string"
#include "includes/glm/glm.hpp"
#include "src/spriteSheet/spriteSheet.hpp"

class Entity
{
public:
    Entity(const std::shared_ptr<SpriteSheet> spriteSheet);

    std::shared_ptr<SpriteSheet> spriteSheet;

    void setSpriteIndex(const std::string &spriteName);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -5.0f);
    glm::vec3 previousPosition = glm::vec3(0.0f, 0.0f, -5.0f); // Position before the latest simulation step
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    // Collision box, relative to position (the bottom-left corner of the sprite)
    glm::vec2 hitboxOffset = glm::vec2(0.0f, 0.0f);
    glm::vec2 hitboxSize = glm::vec2(1.0f, 1.0f);

    // Contacts found by the latest simulation step
    bool onGround = false;
    bool onCeiling = false;
    bool onWallLeft = false;
    bool onWallRight = false;

    int spriteIndex = 0;
};