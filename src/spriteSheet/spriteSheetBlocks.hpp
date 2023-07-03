#pragma once
#include "src/spriteSheet/spriteSheet.hpp"
#include "string"

class SpriteSheetBlocks : public SpriteSheet
{
public:
    SpriteSheetBlocks(const std::string file, const float spriteWidth, const float spriteHeight, const float quadSizeWidth, const float quadSizeHeight);
};