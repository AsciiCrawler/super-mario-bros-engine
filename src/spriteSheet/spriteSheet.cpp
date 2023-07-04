#include "src/spriteSheet/spriteSheet.hpp"
#include "includes/stb_image.h"
#include "includes/glad/glad.h"
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"
#include "src/gameManager.hpp"
#include "iostream"

SpriteSheet::SpriteSheet(const std::string file, const float spriteWidth, const float spriteHeight,  const float quadSizeWidth, const float quadSizeHeight)
{
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);

    int widthPixels, heightPixels, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file.c_str(), &widthPixels, &heightPixels, &nrChannels, 0);

    int spritesW = widthPixels / 18;
    int spritesH = heightPixels / 34;
    float textureWidth = 1.0f / (((float)widthPixels) / spriteWidth);
    float textureHeight = 1.0f / (((float)heightPixels) / spriteHeight);

    float textureWidthPadding = 1.0f / (((float)widthPixels) / (spriteWidth + 2.0f));
    float textureHeightPadding = 1.0f / (((float)heightPixels) / (spriteHeight + 2.0f));
    textureWidthPadding -= textureWidth;
    textureWidthPadding /= 2.0f;
    textureHeightPadding -= textureHeight;
    textureHeightPadding /= 2.0f;

    // Setup SpriteSheet
    for (size_t y = 0; y < spritesH; y++)
    {
        for (size_t x = 0; x < spritesW; x++)
        {
            // Top Right
            this->vertices.push_back(quadSizeWidth);
            this->vertices.push_back(quadSizeHeight);
            this->vertices.push_back(((float(x)) * textureWidth) + textureWidth + textureWidthPadding + (textureWidthPadding * x * 2));
            this->vertices.push_back(((float(y)) * textureHeight) + textureHeight + textureHeightPadding + (textureHeightPadding * y * 2));

            // Bottom Right
            this->vertices.push_back(quadSizeWidth);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + textureWidth + textureWidthPadding + (textureWidthPadding * x * 2));
            this->vertices.push_back(((float(y)) * textureHeight) + 0.0f + textureHeightPadding + (textureHeightPadding * y * 2));

            // Top Left
            this->vertices.push_back(0.0f);
            this->vertices.push_back(quadSizeHeight);
            this->vertices.push_back(((float(x)) * textureWidth) + 0.0f + textureWidthPadding + (textureWidthPadding * x * 2));
            this->vertices.push_back(((float(y)) * textureHeight) + textureHeight + textureHeightPadding + (textureHeightPadding * y * 2));

            /*  */

            // Bottom Right
            this->vertices.push_back(quadSizeWidth);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + textureWidth + textureWidthPadding + (textureWidthPadding * x * 2));
            this->vertices.push_back(((float(y)) * textureHeight) + 0.0f + textureHeightPadding + (textureHeightPadding * y * 2));

            // Bottom Left
            this->vertices.push_back(0.0f);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + 0.0f + textureWidthPadding + (textureWidthPadding * x * 2));
            this->vertices.push_back(((float(y)) * textureHeight) + 0.0f + textureHeightPadding + (textureHeightPadding * y * 2));

            // Top Left
            this->vertices.push_back(0.0f);
            this->vertices.push_back(quadSizeHeight);
            this->vertices.push_back(((float(x)) * textureWidth) + 0.0f + textureWidthPadding + (textureWidthPadding * x * 2));
            this->vertices.push_back(((float(y)) * textureHeight) + textureHeight + textureHeightPadding + (textureHeightPadding * y * 2));
        }
    }

    if (!data)
        std::cout << "Failed to load texture" << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthPixels, heightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void SpriteSheet::use()
{
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}