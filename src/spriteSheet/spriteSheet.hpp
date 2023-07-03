#pragma once
#include "vector"
#include "string"
#include "unordered_map"

class SpriteSheet
{
public:
    SpriteSheet(const std::string file, const float spriteWidth, const float spriteHeight);

    std::vector<float> vertices;
    unsigned int VAO, VBO;
    unsigned int texture;

    std::unordered_map<std::string, int> spriteIndexes;
    void use();
};