//
// Created by Alexander Linnik on 24/12/2019.
//

#include "camera.h"
#include <GLFW/glfw3.h>
#include "old_main.cpp"


const GLfloat Camera::deltaTimeDefault = 0.0f;
const GLfloat Camera::lastFrameDefault = 0.0f;  	// Время вывода последнего кадра
const GLfloat Camera::lastXDefault = 400;
const GLfloat Camera::lastYDefault = 300;
const GLfloat Camera::yawDefault   = -90.0f;
const GLfloat Camera::pitchDefault = 0.0f;
const GLboolean Camera::firstMouseDefault = GL_TRUE;

const glm::vec3 Camera::cameraPosDefault   = glm::vec3(0.0f, 0.0f,  3.0f);
const glm::vec3 Camera::cameraFrontDefault = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Camera::cameraUpDefault    = glm::vec3(0.0f, 1.0f,  0.0f);




Camera::Camera (GLboolean *keys,
                glm::vec3 cameraPos,
                glm::vec3 cameraFront,
                glm::vec3 cameraUp,
                GLfloat deltaTime,
                GLfloat lastFrame,
                GLfloat lastX,
                GLfloat lastY,
                GLfloat yaw,
                GLfloat pitch,
                GLboolean firstMouse) {

    this->keys = keys;
    this->cameraPos = cameraPos;
    this->cameraFront = cameraFront;
    this->cameraUp = cameraUp;

    this->deltaTime = deltaTime;
    this->lastFrame = lastFrame;
    this->lastX = lastX;
    this->lastY = lastY;
    this->yaw = yaw;
    this->pitch = pitch;
    this->firstMouse = firstMouse;
}


void Camera::mouse_callback(const GLFWwindow* window, double xpos, double ypos)
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