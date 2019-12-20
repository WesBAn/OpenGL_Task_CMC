//
// Created by Alexander Linnik on 21/12/2019.
//

#include "mc_shaders.h"
#include <string>
#include <utility>
#include <GL/glew.h>
#include <fstream>

Shaders::Shaders(const std::string &vertex_shader_name, const std::string &fragment_shader_name)
{
    this->vertex_shader_name = vertex_shader_name;
    this->fragment_shader_name = fragment_shader_name;
}


GLuint Shaders::getShaderFromFile(GLenum type, const std::string &filename)
{
    std::ifstream fs(filename);
    if (!fs.is_open())
    {
        std::cerr << "Can't open file" << std::endl;
        return -1;
    }

    std::string shaderText((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    GLuint shaderObj = glCreateShader(type);
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