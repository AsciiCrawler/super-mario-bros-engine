#pragma once
#include "string"
#include "fstream"
#include "iostream"
#include "includes/glad/glad.h"
#include "memory"
#include "src/camera.hpp"

class GameManager
{
public:
    static bool isGameRunning; // Default True
    
    static GLuint vertexShader;   // Set -> openGLToolkit
    static GLuint fragmentShader; // Set -> openGLToolkit
    static GLuint shaderProgram;  // Set -> openGLToolkit
    static std::unique_ptr<Camera> camera;
};