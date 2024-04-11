#include "src/openGLToolkit.hpp"

void OpenGLToolkit::LoadVertexShader()
{
    std::string vertexShaderString = Utilities::readFile("assets/shaders/vertex.vert");
    const char *vertexShaderChars = vertexShaderString.c_str();
    GameManager::vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(GameManager::vertexShader, 1, &vertexShaderChars, nullptr);
    glCompileShader(GameManager::vertexShader);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(GameManager::vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(GameManager::vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }
}

void OpenGLToolkit::LoadFragmentShader()
{
    std::string fragmentShaderString = Utilities::readFile("assets/shaders/fragment.frag");
    const char *fragmentShaderChars = fragmentShaderString.c_str();
    GameManager::fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(GameManager::fragmentShader, 1, &fragmentShaderChars, nullptr);
    glCompileShader(GameManager::fragmentShader);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(GameManager::fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(GameManager::fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }
}

void OpenGLToolkit::LoadShaderProgram()
{
    GameManager::shaderProgram = glCreateProgram();
    glAttachShader(GameManager::shaderProgram, GameManager::vertexShader);
    glAttachShader(GameManager::shaderProgram, GameManager::fragmentShader);
    glLinkProgram(GameManager::shaderProgram);

    glDeleteShader(GameManager::vertexShader);
    glDeleteShader(GameManager::fragmentShader);
}