#include "shader_lib.h"
#include "Model.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct ObjectTransform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 pivotPoint = glm::vec3(0.0f); 
    struct {
        float min = -1.0f;
        float max = 1.0f;
    } zLimit;

    struct {
        float min = -45.0f;
        float max = 45.0f;
    } yRotationLimit;

    struct {
        float min = -15.0f;
        float max = 15.0f;
    } xRotationLimit;
};

std::vector<ObjectTransform> objectTransforms;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

glm::mat4 calculateModelMatrix(int index) {
    glm::mat4 model = glm::mat4(1.0f);

    if (index == 0) {
        return model;
    }

    if (index == 1) {
        model = glm::translate(model, objectTransforms[1].pivotPoint);
        model = glm::rotate(model, glm::radians(objectTransforms[1].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, -objectTransforms[1].pivotPoint);
        return model;
    }

    if (index == 2) {
        model = glm::translate(model, objectTransforms[1].pivotPoint);
        model = glm::rotate(model, glm::radians(objectTransforms[1].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, -objectTransforms[1].pivotPoint);

        model = glm::translate(model, objectTransforms[2].pivotPoint);
        model = glm::rotate(model, glm::radians(objectTransforms[2].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, -objectTransforms[2].pivotPoint);
        return model;
    }

    if (index == 3) {
        model = glm::translate(model, objectTransforms[1].pivotPoint);
        model = glm::rotate(model, glm::radians(objectTransforms[1].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, -objectTransforms[1].pivotPoint);

        model = glm::translate(model, objectTransforms[2].pivotPoint);
        model = glm::rotate(model, glm::radians(objectTransforms[2].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, -objectTransforms[2].pivotPoint);

        model = glm::translate(model, objectTransforms[3].position);
        return model;
    }

    return model;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Model", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");
    Model ourModel("Var_1.obj");

    objectTransforms.resize(4);

    objectTransforms[1].pivotPoint = glm::vec3(3.8312f, 0.5f, -4.75f);

    objectTransforms[2].pivotPoint = glm::vec3(3.7608f, 4.2731f, -4.3605f);

    objectTransforms[1].yRotationLimit = { -45.0f, 45.0f };
    objectTransforms[2].xRotationLimit = { -7.0f, 15.0f };
    objectTransforms[3].zLimit = { -3.5f, 0.5f };

    shader.use();
    shader.setVec3("light.position", glm::vec3(-11.2f, 5.0f, -20.0f));
    shader.setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.setVec3("light.diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
    shader.setVec3("light.specular", glm::vec3(1.0f));
    shader.setVec3("material.ambient", glm::vec3(0.3825f, 0.147f, 0.045f));
    shader.setVec3("material.diffuse", glm::vec3(0.7038f, 0.27048f, 0.0828f));
    shader.setVec3("material.specular", glm::vec3(0.256777f, 0.137622f, 0.086014f));
    shader.setFloat("material.shininess", 128.0f);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setVec3("viewPos", cameraPos);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        for (size_t i = 0; i < ourModel.meshTransforms.size(); ++i) {
            ourModel.meshTransforms[i] = calculateModelMatrix(i);
        }

        ourModel.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 20.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    float rotateSpeed = 50.0f * deltaTime;
    float moveSpeed = 1.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        objectTransforms[1].rotation.y += rotateSpeed;
        if (objectTransforms[1].rotation.y > objectTransforms[1].yRotationLimit.max)
            objectTransforms[1].rotation.y = objectTransforms[1].yRotationLimit.max;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        objectTransforms[1].rotation.y -= rotateSpeed;
        if (objectTransforms[1].rotation.y < objectTransforms[1].yRotationLimit.min)
            objectTransforms[1].rotation.y = objectTransforms[1].yRotationLimit.min;
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        objectTransforms[2].rotation.x += rotateSpeed;
        if (objectTransforms[2].rotation.x > objectTransforms[2].xRotationLimit.max)
            objectTransforms[2].rotation.x = objectTransforms[2].xRotationLimit.max;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        objectTransforms[2].rotation.x -= rotateSpeed;
        if (objectTransforms[2].rotation.x < objectTransforms[2].xRotationLimit.min)
            objectTransforms[2].rotation.x = objectTransforms[2].xRotationLimit.min;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        objectTransforms[3].position.z += moveSpeed;
        if (objectTransforms[3].position.z > objectTransforms[3].zLimit.max)
            objectTransforms[3].position.z = objectTransforms[3].zLimit.max;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        objectTransforms[3].position.z -= moveSpeed;
        if (objectTransforms[3].position.z < objectTransforms[3].zLimit.min)
            objectTransforms[3].position.z = objectTransforms[3].zLimit.min;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}