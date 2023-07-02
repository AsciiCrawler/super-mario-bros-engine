#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#include "includes/SDL2/SDL.h"
#include "includes/glad/glad.h"
#include "includes/stb_image.h"

#include "string"
#include "memory"
#include <typeinfo>
#include <typeindex>

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"

#include "iostream"

#include "src/utilities.hpp"

SDL_Window *window;
SDL_GLContext openGLContext;

void createWindow()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC);
    /* SDL_WarpMouseInWindow(window, 800 / 2, 600 / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);            */

    openGLContext = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

int main(int ArgCount, char **Args)
{
    createWindow();

    /* Load Vertex Shader */
    std::string vertexShaderString = Utilities::readFile("assets/shaders/shader.vert");
    const char *vertexShaderChars = vertexShaderString.c_str();
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderChars, nullptr);
    glCompileShader(vertexShader);

    /* Load Fragment Shader */
    std::string fragmentShaderString = Utilities::readFile("assets/shaders/fragment.frag");
    const char *fragmentShaderChars = fragmentShaderString.c_str();
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderChars, nullptr);
    glCompileShader(fragmentShader);

    /* ShaderProgram */
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* VAO - Vertex array object */
    /* VBO - vertex buffer objects  */
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    /*  */

    /* glEnable(GL_DEPTH_TEST); */
    glUseProgram(shaderProgram);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    SDL_GL_SetSwapInterval(0);

    float lastTick = SDL_GetTicks();
    float milli = 0.0f;
    float lastTime = 0.0f;
    float nFrames = 0.0f;
    float deltaTime = 0.0f;
    bool isGameRunning = true;
    while (isGameRunning)
    {
        milli = SDL_GetTicks() - lastTick;
        lastTick = SDL_GetTicks();
        deltaTime = milli * 0.001f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Render

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(window); // Render

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isGameRunning = false;
                    break;
                }
            }
        }

        ++nFrames;
        if (SDL_GetTicks() > lastTime)
        {
            float ms = (nFrames > 0 ? 1000.0f / nFrames : 0.0f);
            std::string performanceInfoString;

            performanceInfoString += "FPS: " + std::to_string(int(ms > 0 ? 1000 / ms : 0)) + "\tms: " + std::to_string(ms);
            const char *performanceInfoChars = performanceInfoString.c_str();

            SDL_SetWindowTitle(window, ms > 0 ? performanceInfoChars : "fps = 0 ms = 0");

            nFrames = 0;
            lastTime += 1000;
        }
    }

    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}