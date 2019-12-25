//
// Created by Alexander Linnik on 25/12/2019.
//

#ifndef MAIN_MATERIALS_H
#define MAIN_MATERIALS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mc_shaders.h"

struct Materials {
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};
    GLfloat shininess;

    GLint locations[4]{};

    Materials(glm::vec3 ambient,
              glm::vec3 diffuse,
              glm::vec3 specular,
              GLfloat shininess,
              Shaders &shader,
              const char *materialNames[] = nullptr);

    void setUniforms();
};

struct Lights {
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};
    glm::vec3 position{};

    GLint locations[4]{};

    Lights(glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular,
           glm::vec3 position,
           Shaders &lampShader,
           const char *lampNames[] = nullptr);

    void setUniforms();
};



#endif //MAIN_MATERIALS_H
