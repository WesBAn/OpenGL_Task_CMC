//
// Created by Alexander Linnik on 24/12/2019.
//

#ifndef MAIN_CAMERA_H
#define MAIN_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>





class Camera {
    static const GLfloat deltaTimeDefault;
    static const GLfloat lastFrameDefault;  	// Время вывода последнего кадра
    static const GLfloat lastXDefault;
    static const GLfloat lastYDefault;
    static const GLfloat yawDefault;
    static const GLfloat pitchDefault;
    static const GLboolean firstMouseDefault;
    static const glm::vec3 cameraPosDefault;
    static const glm::vec3 cameraFrontDefault;
    static const glm::vec3 cameraUpDefault;

    glm::vec3 cameraPos = glm::vec3(0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f);

    GLfloat deltaTime = 0.0f;
    GLfloat lastX = 0.0f;
    GLfloat lastY = 0.0f;
    GLfloat yaw = 0.0f;
    GLfloat pitch = 0.0f;
    GLboolean firstMouse = GL_FALSE;
    GLfloat lastFrame = 0.0f;
    GLboolean *keys;

    glm::mat4 view = glm::mat4(1.0f);

public:
    explicit Camera (GLboolean *keys,
                     glm::vec3 cameraPos = cameraPosDefault,
                     glm::vec3 cameraFront = cameraFrontDefault,
                     glm::vec3 cameraUp = cameraUpDefault,
                     GLfloat deltaTime = deltaTimeDefault,
                     GLfloat lastFrame = lastFrameDefault,
                     GLfloat lastX = lastXDefault,
                     GLfloat lastY = lastYDefault,
                     GLfloat yaw = yawDefault,
                     GLfloat pitch = pitchDefault,
                     GLboolean firstMouse = firstMouseDefault);
    void mouse_callback(const GLFWwindow* window, double xpos, double ypos);
    void doMovement();
    void cameraPollFrame();
    glm::mat4 getView();
};



#endif //MAIN_CAMERA_H
