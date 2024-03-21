#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// ImGui Windows
#include "imgui_windows/ControlsWindow.h"

#include "InputManager.h"
#include "Shader.h"
#include "Camera.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "tiny_gltf.h"
#include "TextureStore.h"
#include "MaterialStore.h"
#include "ModelStore.h"
#include "GltfModelImporter.h"
#include "ModelGenerator.h"


#include <tracy/Tracy.hpp>

float maxAniso;

// Window dimensions
const GLint WIDTH = 1280;
const GLint HEIGHT = 960;

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height);
void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

InputManager inputManager;

Camera camera{0.05f, 0.3f, glm::vec3{0.0f, 3.0f, 0.0f}, false};
bool resetMousePos = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool grabMouseInput = true;

int main() {
    TextureStore textureStore;
    MaterialStore materialStore;
    ModelStore modelStore;

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
    glEnable(GL_DEPTH_TEST);

    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GltfModelImporter gltfModelImporter(textureStore, materialStore, modelStore);

    gltfModelImporter.uploadModelDirectory("../assets/models/");
    ModelGenerator::generateFloor("floor", 128, 128, util::loadImage("../assets/textures/Asphalt024A_1K-JPG_Color.jpg"), maxAniso, true, textureStore, materialStore, modelStore);

    RenderQueue renderQueue{"scene"};

    renderQueue
    .addModel({
        modelStore.getModel("floor")
    })
    .addModel({
        modelStore.getModel("chair"),
        glm::vec3{0.0f, 0.0f, 0.0f},
        glm::vec3{10.0f}
    })
    .addModel({
        modelStore.getModel("Lantern"),
        glm::vec3{4.0f, 12.5f, 4.0f}
    });

    // ---- ImGui ----
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    // ----------------

    Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");
    shader.use();

    Renderer renderer{shader};
    renderer.addQueue(renderQueue);

    shader.setInt("baseColor", 0);

    // Wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ImGui window instances
    ControlsWindow controlsWindow;

    // Loop until window closed
    while (!glfwWindowShouldClose(window)) {
        ZoneScopedN("Render loop");
        // ---- ImGui ----
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow();
        // ---------------

        controlsWindow.render();

        // Per frame time logic
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        // Get & handle user input events
        processInput(window);

        // Clear window
        glClearColor(0.8f, 0.8f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        shader.use();

        float timeValue = glfwGetTime();
        float scaleValue = sin(timeValue * 2) / 2.0f + 1.0f;
        shader.setFloat("colorScale", scaleValue);

        // MVP
        glm::mat4 projection = glm::perspective(glm::radians(camera.getCurrentFov()), float(WIDTH) / HEIGHT, 0.1f, 100.0f);

        camera.update(deltaTime);
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat4("model", glm::identity<glm::mat4>());

        renderer.renderAllQueues();

        // ---- ImGui ----
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // --------------

        inputManager.resetKeyPresses();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (inputManager.wasKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (inputManager.wasKeyPressed(GLFW_KEY_E)) {
        grabMouseInput = !grabMouseInput;

        if (grabMouseInput) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            resetMousePos = true;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (inputManager.isKeyDown(GLFW_KEY_SPACE)) {
        camera.enqueueDirection(Camera::Direction::UP);
    }

    if (inputManager.isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        camera.enqueueDirection(Camera::Direction::DOWN);
    }

    if (inputManager.isKeyDown(GLFW_KEY_W)) {
        camera.enqueueDirection(Camera::Direction::FORWARD);
    }

    if (inputManager.isKeyDown(GLFW_KEY_S)) {
        camera.enqueueDirection(Camera::Direction::BACKWARD);
    }

    if (inputManager.isKeyDown(GLFW_KEY_A)) {
        camera.enqueueDirection(Camera::Direction::LEFT);
    }

    if (inputManager.isKeyDown(GLFW_KEY_D)) {
        camera.enqueueDirection(Camera::Direction::RIGHT);
    }
}

void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn) {
    if (!grabMouseInput) {
        return;
    }

    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (resetMousePos) {
        lastX = xPos;
        lastY = yPos;
        resetMousePos = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // Reversed y-coords bottom to top
    lastX = xPos;
    lastY = yPos;

    camera.handleMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    if (!grabMouseInput) {
        return;
    }

    camera.handleZoom(static_cast<float>(yOffset));
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        inputManager.setKeyState(key, true);
    }

    if (action == GLFW_RELEASE) {
        inputManager.setKeyState(key, false);
    }
}