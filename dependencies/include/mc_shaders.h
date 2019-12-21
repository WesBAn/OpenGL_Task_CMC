#ifndef MAIN_MC_SHADERS_H
#define MAIN_MC_SHADERS_H

#include <string>
#include <GL/glew.h>

class Shaders {
    static const GLuint INFO_LOG_BUFFER_SIZE;
    std::string vertexShaderName;
    std::string fragmentShaderName;

    GLuint vertexShader;
    GLuint fragmentShader;
public:
    explicit Shaders(const std::string &vertexShaderName, const std::string &fragmentShaderName);

    static GLuint getShaderFromFile(GLenum type, const std::string &filename);
    static GLuint getShaderProgram(GLuint vertShader, GLuint fragmentShader);

    GLboolean shaderCreatingStatus;
    GLuint shaderProgram;

};

#endif //MAIN_MC_SHADERS_H
