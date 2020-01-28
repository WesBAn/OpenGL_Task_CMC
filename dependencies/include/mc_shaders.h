#ifndef MAIN_MC_SHADERS_H
#define MAIN_MC_SHADERS_H

#include <string>
#include <GL/glew.h>


class Shaders {
private:
    static const GLuint INFO_LOG_BUFFER_SIZE;
    std::string vertexShaderName;
    std::string fragmentShaderName;

    GLuint vertexShader;
    GLuint fragmentShader;

public:
    GLboolean shaderCreatingStatus;
    GLuint shaderProgram;
    explicit Shaders(const std::string &vertexShaderName, const std::string &fragmentShaderName);

    template <typename T>
    static GLuint getShaderFromFile(GLenum type, const T &filename);

    static GLuint getShaderProgram(GLuint vertShader, GLuint fragmentShader);

    void use() const;
};

#endif //MAIN_MC_SHADERS_H
