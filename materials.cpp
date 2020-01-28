//
// Created by Alexander Linnik on 25/12/2019.
//
#include "materials.h"

#include <iostream>
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
    this->textureMode = GL_FALSE;
    for (int i = 0; i < 4; ++i) {
        this->locations[i] = glGetUniformLocation(shader.shaderProgram, materialNames[i]);
    }
}

Materials::Materials(GLfloat shininess,
                     Shaders &shader,
                     const char *materialNames[]) {
    this->shininess = shininess;
    this->textureMode = GL_TRUE;

    for (int i = 0; i < 3; ++i) {
        this->locations[i] = glGetUniformLocation(shader.shaderProgram, materialNames[i]);
    }
}

void Materials::setUniforms() {
    if (!textureMode) {
        glUniform3f(locations[0], ambient.x, ambient.y, ambient.z);
        glUniform3f(locations[1], diffuse.x, diffuse.y, diffuse.z);
        glUniform3f(locations[2], specular.x, specular.y, specular.z);
        glUniform1f(locations[3], shininess);
    } else {
        glUniform1i(locations[0], 1);
        glUniform1f(locations[1], shininess);
        glUniform1i(locations[2], 0);
    }
}


Lights::Lights(glm::vec3 ambient,
               glm::vec3 diffuse,
               glm::vec3 specular,
               glm::vec3 position,
               GLfloat constant,
               GLfloat linear,
               GLfloat quadratic,
               Shaders &lampShader,
               const char *lampNames[]) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->sunMode = GL_FALSE;
    for (int i = 0; i < 7; ++i) {
        this->locations[i] = glGetUniformLocation(lampShader.shaderProgram, lampNames[i]);
    }
}

Lights::Lights(glm::vec3 ambient,
               glm::vec3 diffuse,
               glm::vec3 specular,
               glm::vec3 direction,
               Shaders &lampShader,
               const char *lampNames[]) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->direction = direction;
    this->sunMode = GL_TRUE;

    for (int i = 0; i < 4; ++i) {
        this->locations[i] = glGetUniformLocation(lampShader.shaderProgram, lampNames[i]);
    }
}

void Lights::setUniforms() {
    if (sunMode) {
        glUniform3f(locations[0], ambient.x, ambient.y, ambient.z);
        glUniform3f(locations[1], diffuse.x, diffuse.y, diffuse.z);
        glUniform3f(locations[2], specular.x, specular.y, specular.z);
        glUniform3f(locations[3], direction.x, direction.y, direction.z);
    } else {
        glUniform3f(locations[0], ambient.x, ambient.y, ambient.z);
        glUniform3f(locations[1], diffuse.x, diffuse.y, diffuse.z);
        glUniform3f(locations[2], specular.x, specular.y, specular.z);
        glUniform3f(locations[3], position.x, position.y, position.z);

        glUniform1f(locations[4], constant);
        glUniform1f(locations[5], linear);
        glUniform1f(locations[6], quadratic);

    }
}


