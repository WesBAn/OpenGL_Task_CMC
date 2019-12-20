//
// Created by Alexander Linnik on 21/12/2019.
//

#ifndef MAIN_MC_SHADERS_H
#define MAIN_MC_SHADERS_H
#include <string>
#include <GL/glew.h>

class Shaders {

    std::string vertex_shader_name;
    std::string fragment_shader_name;


public:
    Shaders(const std::string &vertex_shader_name, const std::string &fragment_shader_name);
    GLuint getShaderFromFile(GLenum type, const std::string &filename);
};


#endif //MAIN_MC_SHADERS_H
