//
// Created by Sebastian on 16.02.2024.
//

#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H
#include <iostream>
#include <queue>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "util.h"


class Camera {
public:
    enum class Direction {
        FORWARD, RIGHT, BACKWARD, LEFT, UP, DOWN
    };

    Camera(glm::vec3 cameraPos,
           glm::vec3 cameraFront,
           glm::vec3 cameraUp,
           float mouseSensitivity,
           float camAccel,
           float maxVelocity,
           bool fpsCamera);
    explicit Camera(float mouseSensitivity, float camAccel, bool fpsCamera = false);

    void handleMouseMovement(float deltaX, float deltaY);
    void handleZoom(float deltaY);
    void update(float deltaTime);
    void enqueueDirection(Direction direction);
    glm::mat4 getViewMatrix() const;
    float getCurrentFov() const;
private:
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    bool fpsCamera = false;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;

    std::queue<Direction> keyStrokes;

    glm::vec3 velocity{0.0f};
    float maxVelocity = 1.0f;

    float camAccel = 0.1f;
    float mouseSensitivity = 0.1f;

    void recalculateVectors();
    glm::vec3 calculateDirectionVector();
    glm::vec3 calculateForce(float accel, float frictionFac);
    glm::vec3 calculateVelocityDelta(float deltaTime);
};


#endif //OPENGL_CAMERA_H
