//
// Created by Alexander Linnik on 25/12/2019.
//
#include "materials.h"

Materials::Materials(glm::vec3 ambient,
                     glm::vec3 diffuse,
                     glm::vec3 specular,
                     GLfloat shininess,
                     Shaders &shader,
                     const char *materialNames[]) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;

    for (int i = 0; i < 4; ++i) {
        this->locations[i] = glGetUniformLocation(shader.shaderProgram, materialNames[i]);
    }
}

void Materials::setUniforms() {
    glUniform3f(locations[0], ambient.x, ambient.y, ambient.z);
    glUniform3f(locations[1], diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(locations[2], specular.x, specular.y, specular.z);
    glUniform1f(locations[3], shininess);
}



Lights::Lights(glm::vec3 ambient,
               glm::vec3 diffuse,
               glm::vec3 specular,
               glm::vec3 position,
               Shaders &lampShader,
               const char *lampNames[]) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->position = position;

    for (int i = 0; i < 4; ++i) {
        this->locations[i] = glGetUniformLocation(lampShader.shaderProgram, lampNames[i]);
    }
}

void Lights::setUniforms() {
    glUniform3f(locations[0], ambient.x, ambient.y, ambient.z);
    glUniform3f(locations[1], diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(locations[2], specular.x, specular.y, specular.z);
    glUniform3f(locations[3], position.x, position.y, position.z);

}


