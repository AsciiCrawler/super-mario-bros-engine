#pragma once
#include "memory"
#include "vector"
#include "includes/glm/glm.hpp"
#include "src/spriteSheet/spriteSheet.hpp"

// Collects sprites from one sprite sheet in world space and draws them with a single call
class SpriteBatch
{
public:
    SpriteBatch(const std::shared_ptr<SpriteSheet> spriteSheet, const unsigned int usage);
    ~SpriteBatch();
    SpriteBatch(const SpriteBatch &) = delete;
    SpriteBatch &operator=(const SpriteBatch &) = delete;

    std::shared_ptr<SpriteSheet> spriteSheet;

    void clear();
    void add(const int spriteIndex, const glm::vec3 &position);
    void upload();
    void draw() const;

private:
    unsigned int VAO = 0, VBO = 0;
    unsigned int usage; // GL_STATIC_DRAW when uploaded once, GL_STREAM_DRAW when refilled every frame
    std::vector<float> vertices;
    int vertexCount = 0; // Vertices in the GPU buffer
};
