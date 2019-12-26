#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>

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
    WINDOW_HEIGHT = 800,
    SKYBOX_FACES_AMOUNT = 6
};

enum LIGHT_MODES {
    SUN_LIGHT_MODE = 0,
    DIRECT_LIGHT_MODE = 1,
    FULL_MODE = 2
};


static const char *WINDOW_TITLE = "Alexander Linnik";
static const char *VERTEX_SHADER_NAME = "shaders/vert.glsl";
static const char *FRAGMENT_SHADER_NAME = "shaders/fragment.glsl";
static const char *LVERTEX_SHADER_NAME = "shaders/lampvert.glsl";
static const char *LFRAGMENT_SHADER_NAME = "shaders/lampfragment.glsl";
static const char *SKYBOX_VERTEX_SHADER_NAME = "shaders/skyboxvert.glsl";
static const char *SKYBOX_FRAGMENT_SHADER_NAME = "shaders/skyboxfragment.glsl";
static const char *WINDOWS_VERTEX_SHADER_NAME = "shaders/windowvert.glsl";
static const char *WINDOWS_FRAGMENT_SHADER_NAME = "shaders/windowfragment.glsl";
static const char *RESOURCES_DIR = "resources/";
static const char *CONTAINER_FNAME = "container2.png";
static const char *BLICK_FNAME = "BLICK2.png";
static const char *FLOOR_FNAME = "floor1.jpg";
static const char *WINDOW_FNAME = "window.png";


static const char *AME_MAJESTY_PACK[] = {
        "ame_majesty/majesty_rt.tga",
        "ame_majesty/majesty_lf.tga",
        "ame_majesty/majesty_up.tga",
        "ame_majesty/majesty_dn.tga",
        "ame_majesty/majesty_ft.tga",
        "ame_majesty/majesty_bk.tga"
};
const std::string containerPath = std::string(RESOURCES_DIR) + std::string(CONTAINER_FNAME);
const std::string specularMapPath = std::string(RESOURCES_DIR) + std::string(BLICK_FNAME);
const std::string floorPath = std::string(RESOURCES_DIR) + std::string(FLOOR_FNAME);
const std::string windowPath = std::string(RESOURCES_DIR) + std::string(WINDOW_FNAME);


static GLboolean keys[1024];
GLint lightMode = FULL_MODE;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_0) {
        lightMode = (lightMode + 1) % 3;
    } else if(action == GLFW_PRESS) {
        keys[key] = true;
    } else if(action == GLFW_RELEASE) {
        keys[key] = false;
    }
}


inline void setUpGlfwOptions() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS X
}


void checkWindowCreated(GLFWwindow *window) {
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw (-1);
    }
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


GLFWwindow *baseSetUp(GLint &width, GLint &height) {
    glfwInit();
    setUpGlfwOptions();
    // Window set
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          WINDOW_TITLE,
                                          nullptr,
                                          nullptr);
    checkWindowCreated(window);
    setShowingSize(window, 0, 0, width, height);

    //Setting GLEW
    GLint setUpGlewResult = setUpGlew();
    if (setUpGlewResult == -1) throw -1;
    return window;
}


GLuint generateAndDownloadTexture(const char *containerPath, GLboolean floor = GL_FALSE) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (floor) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    int width, height;
    unsigned char* image = SOIL_load_image(containerPath, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

GLuint getWindowTexture(const char *containerPath) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char* image = SOIL_load_image(containerPath, &width, &height, nullptr, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}



GLuint downloadAndSetUpSkybox() {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    const GLint args[SKYBOX_FACES_AMOUNT] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    int width, height, channels;
    for (GLuint i = 0; i < SKYBOX_FACES_AMOUNT; ++i) {
        std::string partName = std::string(RESOURCES_DIR) + std::string(AME_MAJESTY_PACK[i]);
        unsigned char *image = SOIL_load_image(partName.c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
        if (image) {
            std::cout << "LOADED TEXTURE\n";
        }

        glTexImage2D(args[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);

    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    return cubeVAO;
}

GLuint setFloorVAO(GLuint &VBO) {
    GLuint floorVAO;
    glGenVertexArrays(1, &floorVAO);
    setBufferVBO(VBO, planeVertices, sizeof(planeVertices), GL_FALSE);

    glBindVertexArray(floorVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    return floorVAO;
}

GLuint setWindowVAO(GLuint &VBO) {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLfloat*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLfloat*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);
    return VAO;
}


GLuint setLampVAO(GLuint &VBO) {
    GLuint lampVAO;
    glGenVertexArrays(1, &lampVAO);
    setBufferVBO(VBO, vertices, sizeof(vertices), GL_FALSE);

    glBindVertexArray(lampVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return lampVAO;
}


GLuint setSkyboxVAO() {
    GLuint skyboxVBO;
    glGenBuffers(1, &skyboxVBO);

    GLuint skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    return skyboxVAO;
}


Materials setMaterial(Shaders &objShader, GLboolean isTexture=GL_FALSE) {
    if (!isTexture) {
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

        Materials materials(materialAmbient, materialDiffuse, materialSpecular, materialShininess, objShader,
                            materialFieldNames);
        return materials;
    } else {
        GLfloat materialShininess = 32.0f;
        const char *materialFieldNames[] = {
                "material.specular",
                "material.shininess",
                "material.diffuse"
        };

        Materials materials(materialShininess, objShader, materialFieldNames);
        return materials;
    }
}

Lights setSunLights(Shaders &objShader) {
    glm::vec3 sunAmbient(0.1f, 0.1f, 0.1f);
    glm::vec3 sunDiffuse(0.35f, 0.35f, 0.35f);
    glm::vec3 sunSpecular(0.7f, 0.7f, 0.7f);
    glm::vec3 sunDirection(0.0f, -1.0f, 0.3f);
    const char *sunFieldNames[] = {
            "sun.ambient",
            "sun.diffuse",
            "sun.specular",
            "sun.direction"
    };

    Lights lights(sunAmbient, sunDiffuse, sunSpecular, sunDirection, objShader, sunFieldNames);
    return lights;
}


Lights setDirectLights(Shaders &objShader, glm::vec3 lightPos) {
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
    GLfloat constant = 1.0f;
    GLfloat linear = 0.045f;
    GLfloat quadratic = 0.0075f;
    const char *lightsFieldNames[] = {
            "light.ambient",
            "light.diffuse",
            "light.specular",
            "light.position",
            "light.constant",
            "light.linear",
            "light.quadratic",
    };

    Lights lights(lightAmbient, lightDiffuse, lightSpecular, lightPos, constant, linear, quadratic, objShader, lightsFieldNames);
    return lights;
}

void renderSkybox(GLint &skyboxViewLoc,
        GLint &skyboxProjectionLoc,
        Camera &camera,
        Shaders &skyboxShader,
        GLuint &skyboxVAO,
        GLuint &skyboxTexture,
        glm::mat4 projection) {
    glDepthMask(GL_FALSE);
    skyboxShader.use();
    glm::mat4 skyboxView(glm::mat3(camera.getView()));
    glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(skyboxView));
    glUniformMatrix4fv(skyboxProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}


int main() {
    // BASE SET
    GLint width, height;
    GLFWwindow *window = baseSetUp(width, height);

    /// CAMERA AND CALLBACKS
    Camera camera(keys);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Camera::mouse_callback);
    glfwSetKeyCallback(window, keyCallback);

    ///DEPTH
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    ///
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /// Get Shader
    Shaders objShader(VERTEX_SHADER_NAME, FRAGMENT_SHADER_NAME);
    Shaders lampShader(LVERTEX_SHADER_NAME, LFRAGMENT_SHADER_NAME);
    Shaders skyboxShader(SKYBOX_VERTEX_SHADER_NAME, SKYBOX_FRAGMENT_SHADER_NAME);
    Shaders windowsShader(WINDOWS_VERTEX_SHADER_NAME, WINDOWS_FRAGMENT_SHADER_NAME);

    if (!lampShader.shaderCreatingStatus
            or !objShader.shaderCreatingStatus
            or !skyboxShader.shaderCreatingStatus) {
        return -1;
    }

    /// Get textures
    GLuint containerTexture = generateAndDownloadTexture(containerPath.c_str());
    GLuint specularMapTexture = generateAndDownloadTexture(specularMapPath.c_str());
    GLuint floorTexture = generateAndDownloadTexture(floorPath.c_str(), GL_TRUE);
    GLuint skyboxTexture = downloadAndSetUpSkybox();
    GLuint windowTexture = getWindowTexture(windowPath.c_str());

    /// VAO and VBO
    // VBO
    GLuint VBO, floorVBO, windowVBO;
    setBufferVBO(VBO, vertices, sizeof(vertices));
    setBufferVBO(floorVBO, planeVertices, sizeof(planeVertices));
    setBufferVBO(windowVBO, windowVertices, sizeof(windowVertices));


    // VAO
    GLuint cubeVAO = setCubeVAO(VBO);
    GLuint lampVAO = setLampVAO(VBO);
    GLuint skyboxVAO = setSkyboxVAO();
    GLuint floorVAO = setFloorVAO(floorVBO);
    GLuint windowVAO = setWindowVAO(windowVBO);

    /// MATERIALS
    Materials cubesMaterial = setMaterial(objShader, GL_TRUE);    /// MATERIALS
    Lights directLights = setDirectLights(objShader, lightPos);  /// LIGHTS
    Lights sunLights = setSunLights(objShader); /// LIGHTS SUN
    GLint lightModeLoc = glGetUniformLocation(objShader.shaderProgram, "lightMode");

    /// MATRIXES
    // Матрица перспективы
    glm::mat4 projection(1.0f);
    projection = glm::perspective( glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    GLint projectionLoc = glGetUniformLocation(objShader.shaderProgram, "projection");
    GLint lampProjectionLoc = glGetUniformLocation(lampShader.shaderProgram, "projection");
    GLint skyboxProjectionLoc = glGetUniformLocation(skyboxShader.shaderProgram, "projection");
    GLint windowProjectionLoc = glGetUniformLocation(windowsShader.shaderProgram, "projection");

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Матрица модели
    GLint modelLoc = glGetUniformLocation(objShader.shaderProgram, "model");
    GLint lampModelLoc = glGetUniformLocation(lampShader.shaderProgram, "model");
    GLint windowModelLoc = glGetUniformLocation(windowsShader.shaderProgram, "model");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 lampModel(1.0f);
    lampModel = glm::mat4(1.0f);
    lampModel = glm::translate(lampModel, lightPos);
    lampModel = glm::scale(lampModel, glm::vec3(0.2f));

    // Матрица вида
    glm::mat4 view(1.0f);
    GLint viewLoc = glGetUniformLocation(objShader.shaderProgram, "view");
    GLint viewPosLoc = glGetUniformLocation(objShader.shaderProgram, "viewPos");
    GLint lampViewLoc = glGetUniformLocation(lampShader.shaderProgram, "view");
    GLint skyboxViewLoc = glGetUniformLocation(skyboxShader.shaderProgram, "view");
    GLint windowViewLoc = glGetUniformLocation(windowsShader.shaderProgram, "view");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

//    ///NORMAL MAPPING
//    unsigned int depthMapFBO;
//    glGenFramebuffers(1, &depthMapFBO);
//    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
//    unsigned int depthMap;
//    glGenTextures(1, &depthMap);
//    glBindTexture(GL_TEXTURE_2D, depthMap);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
//                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ///Start polling///
    while(!glfwWindowShouldClose(window))
    {
//        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // буфер трафарета не используется
        camera.cameraPollFrame();
        view = camera.getView();
        // OBJ
        objShader.use();

        ///BIND TEXTURES
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, containerTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMapTexture);

        ///SET UNIFORMS
        cubesMaterial.setUniforms();
        sunLights.setUniforms();

        glUniform3f(viewPosLoc, Camera::cameraPos.x, Camera::cameraPos.y, Camera::cameraPos.z);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(lightModeLoc, lightMode);

        glBindVertexArray(cubeVAO);
        for(GLuint i = 0; i < 5; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ///FLOOR
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, floorTexture);

        glBindVertexArray(floorVAO);
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /// LAMP
        if (lightMode != SUN_LIGHT_MODE) {
            directLights.setUniforms();
            lampShader.use();

            glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(lampModel));
            glBindVertexArray(lampVAO);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        ///WINDOWS - not OS
        windowsShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, windowTexture);
        glUniformMatrix4fv(windowViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(windowProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(windowVAO);
        std::map<float, glm::vec3> sorted;
        for (auto windowPos : windowsPos)
        {
            float distance = glm::length(Camera::cameraPos - windowPos);
            sorted[distance] = windowPos;
        }
        for(auto it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            glUniformMatrix4fv(windowModelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }



        renderSkybox(skyboxViewLoc, skyboxProjectionLoc, camera, skyboxShader, skyboxVAO, skyboxTexture, projection);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &windowVAO);
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteVertexArrays(1, &floorVAO);

    glDeleteBuffers(1, &floorVBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &windowVBO);

    glfwTerminate();
    return 0;
}
