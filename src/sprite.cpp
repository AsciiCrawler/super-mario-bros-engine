#include "src/sprite.hpp"
#include "includes/stb_image.h"
#include "includes/glad/glad.h"
#include "iostream"
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"
#include "src/gameManager.hpp"

void Sprite::init()
{
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    /* Texture setup */

    glGenTextures(1, &this->texture1);
    glBindTexture(GL_TEXTURE_2D, this->texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);

    int widthPixels, heightPixels, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("assets/sprites/mario.png", &widthPixels, &heightPixels, &nrChannels, 0);

    std::cout << "Width: " << widthPixels << "\tHeight: " << heightPixels << std::endl;
    std::cout << "Width: " << widthPixels / 18 << "\tHeight: " << heightPixels / 34 << std::endl;

    int spritesW = widthPixels / 18;
    int spritesH = heightPixels / 34;
    float textureWidth = 1.0f / (((float)widthPixels) / 16.0f);
    float textureHeight = 1.0f / (((float)heightPixels) / 32.0f);

    float textureWidthPadding = 1.0f / (((float)widthPixels) / 18.0f);
    float textureHeightPadding = 1.0f / (((float)heightPixels) / 34.0f);
    textureWidthPadding -= textureWidth;
    textureWidthPadding /= 2.0f;
    textureHeightPadding -= textureHeight;
    textureHeightPadding /= 2.0f;

    std::cout << "textureWidth: " << textureWidth << std::endl;
    std::cout << "textureHeight: " << textureHeight << std::endl;
    std::cout << "textureWidthPadding: " << textureWidthPadding << std::endl;
    std::cout << "textureHeightPadding: " << textureHeightPadding << std::endl;

    for (size_t y = 0; y < spritesH; y++)
    {
        for (size_t x = 0; x < spritesW; x++)
        {
            // Top Right
            this->vertices.push_back(1.0f);
            this->vertices.push_back(2.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + textureWidth);
            this->vertices.push_back(((float(y)) * textureHeight) + textureHeight);

            // Bottom Right
            this->vertices.push_back(1.0f);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + textureWidth);
            this->vertices.push_back(((float(y)) * textureHeight) + 0.0f);

            // Top Left
            this->vertices.push_back(0.0f);
            this->vertices.push_back(2.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + 0.0f);
            this->vertices.push_back(((float(y)) * textureHeight) + textureHeight);

            /*  */

            // Bottom Right
            this->vertices.push_back(1.0f);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + textureWidth);
            this->vertices.push_back(((float(y)) * textureHeight) + 0.0f);

            // Bottom Left
            this->vertices.push_back(0.0f);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + 0.0f);
            this->vertices.push_back(((float(y)) * textureHeight) + 0.0f);

            // Top Left
            this->vertices.push_back(0.0f);
            this->vertices.push_back(2.0f);
            this->vertices.push_back(((float(x)) * textureWidth) + 0.0f);
            this->vertices.push_back(((float(y)) * textureHeight) + textureHeight);
        }
    }

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthPixels, heightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Sprite::draw()
{
    {
        const float inverseAspect = 685.0f / 600.0f;
        const float width = (15 * inverseAspect) / 2.0f;
        const float height = 15.0f / 2.0f;

        glm::mat4 projection = glm::ortho(-width, width, -height, height, 0.1f, 100.0f);
        unsigned int projectionLoc = glGetUniformLocation(GameManager::shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }

    {
        const float inverseAspect = 685.0f / 600.0f;
        const float width = (15 * inverseAspect) / 2.0f;
        const float height = 15.0f / 2.0f;

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        unsigned int projectionLoc = glGetUniformLocation(GameManager::shaderProgram, "view");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(view));
    }

    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(this->position, this->position, -6.0f));
        unsigned int modelLoc = glGetUniformLocation(GameManager::shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    }

    this->position += 0.01f;

    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture1);
    glDrawArrays(GL_TRIANGLES, 12, 6);
}