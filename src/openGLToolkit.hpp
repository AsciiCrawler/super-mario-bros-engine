#pragma once
#include "string"
#include "fstream"
#include "iostream"
#include "includes/glad/glad.h"
#include "src/gameManager.hpp"
#include "src/utilities.hpp"

class OpenGLToolkit
{
public:
    static void LoadVertexShader();
    static void LoadFragmentShader();
    static void LoadShaderProgram();
};