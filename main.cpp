#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL2/SOIL2.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mc_shaders.h"
#include "objectPositions.h"
#include "camera.h"
#include "materials.h"

enum SIZES {
    VBO_ARRAY_SIZE = 1,
    WINDOW_WIDTH = 1200,
    WINDOW_HEIGHT = 800
};


static const char *WINDOW_TITLE = "Alexander Linnik";
static const char *VERTEX_SHADER_NAME = "vert.glsl";
static const char *FRAGMENT_SHADER_NAME = "fragment.glsl";
static const char *LVERTEX_SHADER_NAME = "lampvert.glsl";
static const char *LFRAGMENT_SHADER_NAME = "lampfragment.glsl";
static const char *RESOURCES_DIR = "resources/";
static const char *CONTAINER_FNAME = "container.jpg";
static const char *FACE_FNAME = "awesomeface.jpg";

static GLboolean keys[1024];

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}


inline void setUpGlfwOptions() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS X
}


inline GLint setUpGlew() {
    glewExperimental = GL_TRUE;
    int initResult = glewInit();

    return initResult == GLEW_OK ? 0 : -1;
}


inline void setShowingSize(GLFWwindow* window, GLint x, GLint y, GLint &width, GLint &height) {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwMakeContextCurrent(window);
}


void setBufferVBO(GLuint &VBO, const GLfloat *vertices, const GLuint sizeof_vertices, bool firstTime=GL_TRUE) {
    if (firstTime) {
        glGenBuffers(VBO_ARRAY_SIZE, &VBO);
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (firstTime) {
        glBufferData(GL_ARRAY_BUFFER, sizeof_vertices, vertices, GL_STATIC_DRAW);
    }
}


GLuint setCubeVAO(GLuint &VBO) {
    GLuint cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    setBufferVBO(VBO, vertices, sizeof(vertices), GL_FALSE);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    return cubeVAO;
}

GLuint setLampVAO(GLuint &VBO) {
    GLuint lampVAO;
    glGenVertexArrays(1, &lampVAO);
    setBufferVBO(VBO, vertices, sizeof(vertices), GL_FALSE);

    glBindVertexArray(lampVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return lampVAO;
}

void checkWindowCreated(GLFWwindow *window) {
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw (-1);
    }
}

Materials setMaterial(Shaders &objShader) {
    /// MATERIALS MATERIAL
    /// OBJ
    glm::vec3 materialAmbient(1.0f, 0.5f, 0.31f);
    glm::vec3 materialDiffuse(1.0f, 0.5f, 0.31f);
    glm::vec3 materialSpecular(0.5f, 0.5f, 0.5f);
    GLfloat materialShininess = 32.0f;
    const char *materialFieldNames[] = {
            "material.ambient",
            "material.diffuse",
            "material.specular",
            "material.shininess"
    };

    Materials materials(materialAmbient, materialDiffuse, materialSpecular, materialShininess, objShader, materialFieldNames);
    return materials;
}

Lights setLights(Shaders &objShader, glm::vec3 lightPos) {
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
    const char *lightsFieldNames[] = {
            "light.ambient",
            "light.diffuse",
            "light.specular",
            "light.position"
    };

    Lights lights(lightAmbient, lightDiffuse, lightSpecular, lightPos, objShader, lightsFieldNames);
    return lights;
}



int main() {
    const std::string containerPath = std::string(RESOURCES_DIR) + std::string(CONTAINER_FNAME);
    const std::string facePath = std::string(RESOURCES_DIR) + std::string(FACE_FNAME);

    glfwInit();
    setUpGlfwOptions();
    // Window set
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          WINDOW_TITLE,
                                          nullptr,
                                          nullptr);
    checkWindowCreated(window);
    GLint width, height;
    setShowingSize(window, 0, 0, width, height);

    //Setting GLEW
    GLint setUpGlewResult = setUpGlew();
    if (setUpGlewResult == -1) return -1;

    // Get Shader
    Shaders objShader(VERTEX_SHADER_NAME, FRAGMENT_SHADER_NAME);
    if (!objShader.shaderCreatingStatus) return -1;

    Shaders lampShader(LVERTEX_SHADER_NAME, LFRAGMENT_SHADER_NAME);
    if (!lampShader.shaderCreatingStatus) return -1;

    Camera camera(keys);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Camera::mouse_callback);

    glfwSetKeyCallback(window, keyCallback);

    glEnable(GL_DEPTH_TEST);

    /// OBJECTS
    // VBO
    GLuint VBO;
    setBufferVBO(VBO, vertices, sizeof(vertices));

    // cubeVAO
    GLuint cubeVAO = setCubeVAO(VBO);

    // ISTOCHNIK SVETA VAO
    GLuint lampVAO = setLampVAO(VBO);

    //////////////
//    GLint objectColorLoc = glGetUniformLocation(objShader.shaderProgram, "objectColor");

    /// Позиция lightPos
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    /// MATERIALS
    Materials materials = setMaterial(objShader);    /// MATERIALS
    Lights lights = setLights(objShader, lightPos);  /// LIGHTS

    /// MATRIXES
    /// Матрица перспективы
    glm::mat4 projection(1.0f);
    projection = glm::perspective( glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    GLint projectionLoc = glGetUniformLocation(objShader.shaderProgram, "projection");
    GLint lampProjectionLoc = glGetUniformLocation(lampShader.shaderProgram, "projection");

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    /// Матрица модели
    GLint modelLoc = glGetUniformLocation(objShader.shaderProgram, "model");
    GLint lampModelLoc = glGetUniformLocation(lampShader.shaderProgram, "model");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 lampModel(1.0f);
    lampModel = glm::mat4(1.0f);
    lampModel = glm::translate(lampModel, lightPos);
    lampModel = glm::scale(lampModel, glm::vec3(0.2f));

    /// Матрица вида
    glm::mat4 view(1.0f);
    GLint viewLoc = glGetUniformLocation(objShader.shaderProgram, "view");
    GLint lampViewLoc = glGetUniformLocation(lampShader.shaderProgram, "view");

    GLint viewPosLoc = glGetUniformLocation(objShader.shaderProgram, "viewPos");

    //Start polling
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        camera.cameraPollFrame();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        view = camera.getView();

        // OBJ
        objShader.use();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUniform3f(viewPosLoc, Camera::cameraPos.x, Camera::cameraPos.y, Camera::cameraPos.z);
        materials.setUniforms();
        lights.setUniforms();

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // LAMP
        lampShader.use();

        glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(lampModel));

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
