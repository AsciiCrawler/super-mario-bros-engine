#pragma once
#include "vector"

class SpriteSheet
{
public:
    std::vector<float> vertices;
    unsigned int VAO, VBO;
    unsigned int texture;

    void init();
    void use();
    void draw();
};