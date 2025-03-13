#define GLEW_DLL
#define GLFW_DLL

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "shader_lib.h"
#include <cstdio>
#include <cmath>

GLfloat points[] = {
    0.0f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.25f, 0.433f, 0.0f,
    -0.25f, 0.433f, 0.0f,
    -0.5f, 0.0f, 0.0f,
    -0.25f, -0.433f, 0.0f,
    0.25f, -0.433f, 0.0f
};

GLuint index[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 1
};

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "Hexagon", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3.\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ShaderProgram shaderProgram("vertex_shader.glsl", "fragment_shader.glsl");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0, 1.0, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float Red = abs(sin(timeValue));
        float Green = abs(sin(timeValue * 2.0f));
        float Blue = abs(sin(timeValue * 1.5f));

        shaderProgram.use();
        shaderProgram.setUniform("ourColor", Red, Green, Blue, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(index) / sizeof(index[0]), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}