#include <iostream>
#include <string>
#include <format>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils.h"
#include "Shader.h"

// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height);
void processInput(GLFWwindow* window);

int main() {
    // Init GLFW
    if (!glfwInit()) {
        printf("GLFW init failed!");
        glfwTerminate();
        return -1;
    }

    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "My OpenGL App", nullptr, nullptr);

    if (!window) {
        std::cout << "GLFW window creation failed!\n";
        glfwTerminate();

        return -1;
    }

    int bufferWidth, bufferHeight;

    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // GLEW context
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    float vertices[] = {
            // Pos              // Color
            0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
            0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom left
    };
    unsigned int indices[] = {
            0, 1, 2,   // first triangle
    };

    // VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Copy over data (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    GLuint ebo;
    glGenBuffers(1, &ebo);

    // Bind and copy over
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    // Can safely unbind the buffer now, attrib ptr stored in vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind VAO
    glBindVertexArray(0);

    Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");

    // Wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Loop until window closed
    while (!glfwWindowShouldClose(window)) {
        // Get & handle user input events
        processInput(window);

        // Clear window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        // glDrawArrays(GL_TRIANGLES, 0, 3);


        float timeValue = glfwGetTime();
        float scaleValue = sin(timeValue * 3) / 4.0f + 0.75f;
        shader.setFloat("colorScale", scaleValue);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {

}