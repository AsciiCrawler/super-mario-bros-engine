#include "src/spriteSheet/spriteSheetBlocks.hpp"
#include "iostream"

SpriteSheetBlocks::SpriteSheetBlocks(const std::string file, const float spriteWidth, const float spriteHeight, const float quadWidth, const float quadHeight) : SpriteSheet(file, spriteWidth, spriteHeight, quadWidth, quadHeight)
{
    auto getSpriteIndex = [](int index)
    { return index * 6; };

    this->spriteIndexes.insert(std::make_pair<std::string, int>("brick-0", getSpriteIndex(0)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("brick-1", getSpriteIndex(1)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("brick-2", getSpriteIndex(2)));

    this->spriteIndexes.insert(std::make_pair<std::string, int>("block-0", getSpriteIndex(3)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("block-1", getSpriteIndex(4)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("block-2", getSpriteIndex(5)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("block-3", getSpriteIndex(5)));

    this->spriteIndexes.insert(std::make_pair<std::string, int>("coin-0", getSpriteIndex(4)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("coin-1", getSpriteIndex(5)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("coin-2", getSpriteIndex(5)));
}