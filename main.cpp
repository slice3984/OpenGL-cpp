#include <iostream>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

int main() {
    // Init GLFW
    if (!glfwInit()) {
        printf("GLFW init failed!");
        glfwTerminate();
        return -1;
    }

    // GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "My OpenGL App", nullptr, nullptr);

    if (!mainWindow) {
        printf("GLFW window creation failed!");
        glfwTerminate();

        return -1;
    }

    int bufferWidth, bufferHeight;

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // GLEW context
    glfwMakeContextCurrent(mainWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to init GLAD");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();

        return -1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) {
        // Get & handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);

    }

    return 0;
}
