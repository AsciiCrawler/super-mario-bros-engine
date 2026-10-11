#include "src/spriteBatch.hpp"
#include "includes/glad/glad.h"

SpriteBatch::SpriteBatch(const std::shared_ptr<SpriteSheet> spriteSheet, const unsigned int usage)
{
    this->spriteSheet = spriteSheet;
    this->usage = usage;

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    SpriteSheet::setupVertexAttributes();
}

SpriteBatch::~SpriteBatch()
{
    glDeleteBuffers(1, &this->VBO);
    glDeleteVertexArrays(1, &this->VAO);
}

void SpriteBatch::clear()
{
    this->vertices.clear();
}

void SpriteBatch::add(const int spriteIndex, const glm::vec3 &position)
{
    // Copy the sprite's 6 vertices from the sheet, moved from quad space to world space
    const float *sprite = this->spriteSheet->vertices.data() + spriteIndex * 4;
    for (int v = 0; v < 6; v++)
    {
        this->vertices.push_back(sprite[v * 4 + 0] + position.x);
        this->vertices.push_back(sprite[v * 4 + 1] + position.y);
        this->vertices.push_back(sprite[v * 4 + 2]);
        this->vertices.push_back(sprite[v * 4 + 3]);
    }
}

void SpriteBatch::upload()
{
    // Allocating fresh storage before writing lets the driver keep the previous
    // frame's buffer in use instead of waiting for the GPU to finish with it
    const GLsizeiptr bytes = sizeof(float) * this->vertices.size();
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, bytes, nullptr, this->usage);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, this->vertices.data());
    this->vertexCount = (int)(this->vertices.size() / 4);
}

void SpriteBatch::draw() const
{
    if (this->vertexCount == 0)
        return;
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->spriteSheet->texture);
    glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
}
