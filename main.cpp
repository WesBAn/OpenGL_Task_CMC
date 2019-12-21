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

enum SIZES {
    VBO_ARRAY_SIZE = 1,
    WINDOW_WIDTH = 800,
    WINDOW_HEIGHT = 600
};


static const char *WINDOW_TITLE = "Alexander Linnik";
static const char *VERTEX_SHADER_NAME = "vert.glsl";
static const char *FRAGMENT_SHADER_NAME = "fragment.glsl";
static const char *RESOURCES_DIR = "resources/";
static const char *CONTAINER_FNAME = "container.jpg";
static const char *FACE_FNAME = "awesomeface.jpg";


inline void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


inline void setUpGlfwOptions() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS X
}


inline GLuint setUpGlew() {
    glewExperimental = GL_TRUE;
    int initResult = glewInit();

    return initResult == GLEW_OK ? 0 : -1;
}


inline void setShowingSize(GLFWwindow* window, GLint x, GLint y, GLint &width, GLint &height) {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwMakeContextCurrent(window);
}


inline void setBufferVBO(GLuint &VBO, const GLfloat *vertices, const GLuint sizeof_vertices) {
    glGenBuffers(VBO_ARRAY_SIZE, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vertices, vertices, GL_STATIC_DRAW);
}


int main() {
    glfwInit();
    setUpGlfwOptions();
    const std::string containerPath = std::string(RESOURCES_DIR) + std::string(CONTAINER_FNAME);
    const std::string facePath = std::string(RESOURCES_DIR) + std::string(FACE_FNAME);

    // Creating window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          WINDOW_TITLE,
                                          nullptr,
                                          nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Setting size
    GLint width, height;
    setShowingSize(window, 0, 0, width, height);

    //Setting GLEW
    GLint setUpGlewResult = setUpGlew();
    if (setUpGlewResult == -1) return -1;

    //Setting callback
    glfwSetKeyCallback(window, keyCallback);

    // Vertical input
//    static const GLfloat vertices[] = {
//            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
//    };
    GLfloat vertices[] = {
            // Позиции          // Цвета             // Текстурные координаты
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
    };
    GLfloat texCoords[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.5f, 1.0f
    };
    GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
    };



    // VBO
    GLuint VBO;
    setBufferVBO(VBO, vertices, sizeof(vertices));

    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    // Get Shader
    Shaders shader(VERTEX_SHADER_NAME, FRAGMENT_SHADER_NAME);
    if (!shader.shaderCreatingStatus) {
        return -1;
    }

    // Texture1 Container
    int containerWidth, containerHeight;
    unsigned char *image1 = SOIL_load_image(containerPath.c_str(), &containerWidth, &containerHeight, 0, SOIL_LOAD_RGB);

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, containerWidth, containerHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image1);
    glBindTexture(GL_TEXTURE_2D, 0);

    int faceWidth, faceHeight;
    unsigned char *image2 = SOIL_load_image(facePath.c_str(), &faceWidth, &faceHeight, 0, SOIL_LOAD_RGB);

    // Texture2 Face
    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Obj transform
    glm::mat4 trans(1.0f);
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // Orthographic
    //    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f );

    // Perspective
    glm::mat4 projection(1.0f);
    projection = glm::perspective( glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    GLint projectionLoc = glGetUniformLocation(shader.shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Матрица модели
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    GLint modelLoc = glGetUniformLocation(shader.shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Матрица вида
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    GLint viewLoc = glGetUniformLocation(shader.shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    //Start polling
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


        // TEXTURE CONTAINER
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shader.shaderProgram, "ourTexture1"), 0);

        // TEXTURE FACE
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(shader.shaderProgram, "ourTexture2"), 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
// glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//    glm::mat4 trans(1.0f);
//    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//    vec = trans * vec;
//    std::cout << vec.x << vec.y << vec.z << std::endl;
