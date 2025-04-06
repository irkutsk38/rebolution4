#define GLEW_DLL
#define GLFW_DLL
#include <cstdio>
#include <math.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "shader_loader.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 768;
GLfloat cameraYaw = -90.0f;
GLfloat cameraPitch = 0.0f;
bool isFirstMouse = true;
float cursorX = WIDTH / 2.0f;
float cursorY = HEIGHT / 2.0f;
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    float x = static_cast<float>(xPos);
    float y = static_cast<float>(yPos);
    if (isFirstMouse) {
        cursorX = x;
        cursorY = y;
        isFirstMouse = false;
    }
    float deltaX = x - cursorX;
    float deltaY = cursorY - y;
    cursorX = x;
    cursorY = y;
    float sens = 0.1f;
    deltaX *= sens;
    deltaY *= sens;
    cameraYaw += deltaX;
    cameraPitch += deltaY;
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
}
int main() {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW initialization failed!\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "luminar", nullptr, nullptr);
    if (!mainWindow) {
        fprintf(stderr, "Window creation failed.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "GLEW initialization failed!\n");
        return -1;
    }
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    GLfloat vertices[] = {
        -0.4f, -0.2f, 0.0f,
        -0.4f,  0.4f, 0.0f,
         0.4f,  0.4f, 0.0f,
         0.4f, -0.2f, 0.0f
    };
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
    Shader_loader shaderUtil;
    GLuint shaderID = shaderUtil.oneLinkProgram();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(0);
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
    while (!glfwWindowShouldClose(mainWindow)) {
        glm::vec3 direction;
        direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        direction.y = sin(glm::radians(cameraPitch));
        direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        camTarget = glm::normalize(direction);
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camPos, camPos + camTarget, camUp);
        glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(mainWindow, mouse_callback);
        float moveSpeed = 0.05f;
        if (glfwGetKey(mainWindow, GLFW_KEY_W) == GLFW_PRESS)
            camPos += moveSpeed * camTarget;
        if (glfwGetKey(mainWindow, GLFW_KEY_S) == GLFW_PRESS)
            camPos -= moveSpeed * camTarget;
        if (glfwGetKey(mainWindow, GLFW_KEY_A) == GLFW_PRESS)
            camPos += glm::normalize(glm::cross(camUp, camTarget)) * moveSpeed;
        if (glfwGetKey(mainWindow, GLFW_KEY_D) == GLFW_PRESS)
            camPos -= glm::normalize(glm::cross(camUp, camTarget)) * moveSpeed;
        glClearColor(0.1f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderID);
        shaderUtil.uniform_set_vec(shaderID, "projection", 1, &proj[0][0], false, shaderUtil.M4);
        shaderUtil.uniform_set_vec(shaderID, "view", 1, &view[0][0], false, shaderUtil.M4);
        float t = glfwGetTime();
        float delta = cos(t) / 15000.0f;
        vertices[0] += delta;
        vertices[1] += delta;
        vertices[3] += delta;
        vertices[4] += delta;
        vertices[6] += delta;
        vertices[7] += delta;
        vertices[9] += delta;
        vertices[10] += delta;
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        shaderUtil.uniform_set_vec(shaderID, "in_color", 1 - sin(t), cos(t), sin(t), 1.0f);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


