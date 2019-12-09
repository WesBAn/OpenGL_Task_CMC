#include <iostream>
#include <fstream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

enum {
    INFO_LOG_BUFFER_SIZE = 512
};


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


inline void setGlfwOptions() {
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


GLuint getShaderFromFile(GLenum type, const std::string &filename)
{
    std::ifstream fs(filename);
    if (!fs.is_open())
    {
        std::cerr << "Can't open file" << std::endl;
        return -1;
    }

    std::string shaderText((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    GLuint shaderObj = glCreateShader(type);
    const char *shaderSrc = shaderText.c_str();
    glShaderSource(shaderObj, 1, &shaderSrc, nullptr);
    glCompileShader(shaderObj);
    GLint compileStatus;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLchar infoLog[INFO_LOG_BUFFER_SIZE];
        glGetShaderInfoLog(shaderObj, INFO_LOG_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader compilation failed : " << std::endl << infoLog << std::endl;
        return -1;
    }

    return shaderObj;
}


GLuint getShaderProgram(const GLuint vertShader, const GLuint fragmentShader) {
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertShader);
    glDeleteShader(fragmentShader);

    GLint success;
    GLchar infoLog[INFO_LOG_BUFFER_SIZE];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, INFO_LOG_BUFFER_SIZE, nullptr, infoLog);
    }
    return shaderProgram;
}


int main() {
    glfwInit();
    setGlfwOptions();

    // Creating window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Alexander Linnik", nullptr, nullptr);
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
    GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Get Shader
    const std::string vertShaderName = "vert.glsl";
    GLuint vertShader = getShaderFromFile(GL_VERTEX_SHADER, vertShaderName);

    const std::string fragmentShaderName = "fragment.glsl";
    GLuint fragmentShader = getShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderName);

    if (vertShader == - 1 || fragmentShader == -1) {
        return -1;
    }

    // Use shaders
    GLuint shaderProgram = getShaderProgram(vertShader, fragmentShader);
    glUseProgram(shaderProgram);



    //Start polling
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}