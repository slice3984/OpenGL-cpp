//
// Created by Sebastian on 16.02.2024.
//

#include "Camera.h"

Camera::Camera(glm::vec3 cameraPos,
               glm::vec3 cameraFront,
               glm::vec3 cameraUp,
               float mouseSensitivity,
               float camAccel,
               float maxVelocity,
               bool fpsCamera) :
        cameraPos{cameraPos},
        cameraFront{cameraFront},
        cameraUp{cameraUp},
        mouseSensitivity{mouseSensitivity},
        camAccel{camAccel},
        maxVelocity{maxVelocity},
        fpsCamera{fpsCamera} {}

Camera::Camera(float mouseSensitivity, float camAccel, bool fpsCamera) : Camera(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        mouseSensitivity,
        camAccel,
        0.5f,
        fpsCamera) {}

Camera::Camera(float mouseSensitivty, float camAccel, glm::vec3 cameraPos, bool fpsCamera) : Camera(
        cameraPos,
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        mouseSensitivty,
        camAccel,
        0.5f,
        fpsCamera
        ) {}

void Camera::handleMouseMovement(float deltaX, float deltaY) {
    deltaX *= mouseSensitivity;
    deltaY *= mouseSensitivity;

    yaw += deltaX;
    pitch += deltaY;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }

    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    recalculateVectors();
}

void Camera::handleZoom(float deltaY) {
    fov -= deltaY;

    if (fov < 1.0f) {
        fov = 1.0f;
    }

    if (fov > 45.0f) {
        fov = 45.0f;
    }
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

float Camera::getCurrentFov() const {
    return fov;
}

void Camera::recalculateVectors() {
    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
}

glm::vec3 Camera::calculateDirectionVector() {
    glm::vec3 dirVec{};

    // Iterate over all stored key presses
    while (!keyStrokes.empty()) {
        switch (keyStrokes.front()) {
            case Direction::UP:
                dirVec += fpsCamera ? glm::vec3{} : glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            case Direction::DOWN:
                dirVec += fpsCamera ? glm::vec3{} : glm::vec3(0.0f, -1.0f, 0.0f);
                break;
            case Direction::FORWARD:
                dirVec += cameraFront;
                break;
            case Direction::BACKWARD:
                dirVec -= cameraFront;
                break;
            case Direction::LEFT:
                dirVec -= glm::normalize(glm::cross(cameraFront, cameraUp));
                break;
            case Direction::RIGHT:
                dirVec += glm::normalize(glm::cross(cameraFront, cameraUp));
                break;
        }

        keyStrokes.pop();
    }

    if (glm::length(dirVec) < 1e-3f) {
        return glm::vec3{};
    }

    if (fpsCamera) {
        dirVec.y = 0.0f;
    }
    return glm::normalize(dirVec);
}

glm::vec3 Camera::calculateForce(float accel, float frictionFac) {
    glm::vec3 dirVec = calculateDirectionVector();
    return accel * dirVec - frictionFac * velocity;
}

glm::vec3 Camera::calculateVelocityDelta(float deltaTime) {
    glm::vec3 force = calculateForce(camAccel, 2.0f);
    return force * deltaTime;
}

void Camera::update(float deltaTime) {
    velocity += calculateVelocityDelta(deltaTime);
    float speed = glm::length(velocity);
    velocity = speed > maxVelocity ? (glm::normalize(velocity) * maxVelocity) : velocity;

    cameraPos += velocity;
}

void Camera::enqueueDirection(Direction direction) {
    keyStrokes.push(direction);
}


