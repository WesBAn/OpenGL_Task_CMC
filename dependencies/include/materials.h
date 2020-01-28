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

    GLboolean textureMode;
    GLint locations[4]{};

    Materials(glm::vec3 ambient,
              glm::vec3 diffuse,
              glm::vec3 specular,
              GLfloat shininess,
              Shaders &shader,
              const char *materialNames[] = nullptr);

    Materials(GLfloat shininess,
              Shaders &shader,
              const char *materialNames[] = nullptr);

    void setUniforms();
};

struct Lights {
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};
    glm::vec3 position{};
    glm::vec3 direction{};

    GLfloat constant = 0.0f;
    GLfloat linear = 0.0f;
    GLfloat quadratic = 0.0f;
    GLint locations[7]{};
    GLboolean sunMode = GL_FALSE;

    Lights(glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular,
           glm::vec3 position,
           GLfloat constant,
           GLfloat linear,
           GLfloat quadratic,
           Shaders &lampShader,
           const char *lampNames[] = nullptr);

    Lights(glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular,
           glm::vec3 direction,
           Shaders &lampShader,
           const char *lampNames[]);

    void setUniforms();
};



#endif //MAIN_MATERIALS_H
