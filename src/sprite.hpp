#pragma once
#include "vector"

class Sprite
{
public:
    std::vector<float> vertices;
    unsigned int VAO, VBO, EBO;
    unsigned int texture1;

    float position;

    void init();
    void draw();
};