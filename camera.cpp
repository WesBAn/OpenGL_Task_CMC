//
// Created by Alexander Linnik on 24/12/2019.
//

#include "camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Camera::cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 Camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);



GLfloat Camera::deltaTime = 0.0f;
GLfloat Camera::lastX = 600;
GLfloat Camera::lastY = 400;
GLfloat Camera::yaw = -90.0f;
GLfloat Camera::pitch = 0.0f;
GLboolean Camera::firstMouse = GL_TRUE;
GLfloat Camera::lastFrame = 0.0f;
GLboolean *Camera::keys = nullptr;


Camera::Camera(GLboolean *keysArg) {
    Camera::keys = keysArg;
}


void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void Camera::doMovement()
{
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;;
    if(keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if(keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if(keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


void Camera::cameraPollFrame() {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    doMovement();
}

glm::mat4 Camera::getView() {
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}