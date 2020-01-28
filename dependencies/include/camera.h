//
// Created by Alexander Linnik on 24/12/2019.
//

#ifndef MAIN_CAMERA_H
#define MAIN_CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
    static GLfloat deltaTime;
    static GLfloat lastX;
    static GLfloat lastY;
    static GLfloat yaw;
    static GLfloat pitch;
    static GLboolean firstMouse;
    static GLfloat lastFrame;
    static GLboolean *keys;

    glm::mat4 view = glm::mat4(1.0f);

public:
    static glm::vec3 cameraPos;
    static glm::vec3 cameraFront;
    static glm::vec3 cameraUp;

    explicit Camera(GLboolean *keys);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void doMovement();
    void cameraPollFrame();
    glm::mat4 getView();
};



#endif //MAIN_CAMERA_H
