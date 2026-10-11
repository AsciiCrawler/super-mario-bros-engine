#include "src/entity.hpp"

Entity::Entity(const std::shared_ptr<SpriteSheet> spriteSheet)
{
    this->spriteSheet = spriteSheet;
}

void Entity::setSpriteIndex(const std::string &spriteName)
{
    this->spriteIndex = this->spriteSheet->spriteIndexes[spriteName];
}