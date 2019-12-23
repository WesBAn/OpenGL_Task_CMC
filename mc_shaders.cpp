#include "mc_shaders.h"

#include <string>
#include <GL/glew.h>
#include <fstream>
#include <iostream>

const GLuint Shaders::INFO_LOG_BUFFER_SIZE = 512;


Shaders::Shaders(const std::string &vertexShaderName, const std::string &fragmentShaderName)
{
    this->vertexShaderName = vertexShaderName;
    this->vertexShader = Shaders::getShaderFromFile(GL_VERTEX_SHADER, vertexShaderName);

    this->fragmentShaderName = fragmentShaderName;
    this->fragmentShader = Shaders::getShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderName);

    if (this->vertexShader == - 1 || this->fragmentShader == -1) {
        this->shaderCreatingStatus = GL_FALSE;
        this->shaderProgram = 0;
    } else {
        this->shaderCreatingStatus = GL_TRUE;
        this->shaderProgram = Shaders::getShaderProgram(this->vertexShader, this->fragmentShader);
    }
}

template <typename T>
GLuint Shaders::getShaderFromFile(GLenum type, const T &filename)
{
    GLuint shaderObj = glCreateShader(type);
    std::ifstream fs(filename);
    if (!fs.is_open())
    {
        std::cerr << "Can't open file" << std::endl;
        return -1;
    }

    std::string shaderText((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    const char *shaderSrc = shaderText.c_str();
    glShaderSource(shaderObj, 1, &shaderSrc, nullptr);
    glCompileShader(shaderObj);
    GLint compileStatus;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLchar infoLog[INFO_LOG_BUFFER_SIZE];
        glGetShaderInfoLog(shaderObj, INFO_LOG_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader compilation failed : " << std::endl << infoLog << std::endl;
        return -1;
    }

    return shaderObj;
}


GLuint Shaders::getShaderProgram(GLuint vertShader, GLuint fragmentShader) {
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertShader);
    glDeleteShader(fragmentShader);

    GLint success;
    GLchar infoLog[INFO_LOG_BUFFER_SIZE];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, INFO_LOG_BUFFER_SIZE, nullptr, infoLog);
    }
    return shaderProgram;
}


void Shaders::use() const{
    glUseProgram(this->shaderProgram);
}