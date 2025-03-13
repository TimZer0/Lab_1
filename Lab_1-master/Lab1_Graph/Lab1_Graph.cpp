//14 вариант

#define GLEW_DLL
#define GLFW_DLL

#include "GL/glew.h" 
#include "GLFW/glfw3.h" 
#include <cstdio>
#include <cmath>

#define PI 3.14159265358979323846

int main()
{
    printf("Hello World!\n");

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* windo;
    windo = glfwCreateWindow(512, 512, "Hexagon", NULL, NULL);

    if (!windo) {
        fprintf(stderr, "ERROR: could not open window with GLFW3.\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(windo);

    glewExperimental = GL_TRUE;
    glewInit();

    while (!glfwWindowShouldClose(windo))
    {
        glClearColor(1.0, 1.0, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.5, 0.5, 1.0);
        glBegin(GL_POLYGON);

        for (int i = 0; i < 6; ++i) {
            float angle = 2.0f * PI * i / 6;
            float x = 0.5f * cos(angle);
            float y = 0.5f * sin(angle);
            glVertex2f(x, y);
        }

        glEnd();

        glfwSwapBuffers(windo);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}