#pragma once
#include "src/spriteSheet/spriteSheet.hpp"
#include "string"

class SpriteSheetMario : public SpriteSheet
{
public:
    SpriteSheetMario(const std::string file, const float spriteWidth, const float spriteHeight, const float quadSizeWidth, const float quadSizeHeight);
};