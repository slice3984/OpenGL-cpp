//
// Created by Sebastian on 23.02.2024.
//

#ifndef OPENGL_RENDERENTITY_H
#define OPENGL_RENDERENTITY_H

#include <memory>
#include <utility>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GPUTexturedMesh.h"

class RenderEntity {
private:
    std::vector<GPUTexturedMesh> GPUModelObjs{};
    glm::mat4 modelMatrix = glm::identity<glm::mat4>();
    glm::mat4 translationMatrix = glm::identity<glm::mat4>();
    glm::mat4 rotationMatrix = glm::identity<glm::mat4>();
    glm::mat4 scaleMatrix = glm::identity<glm::mat4>();

    void updateModelMatrix() {
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    }

public:
    // Constructors
    RenderEntity(std::vector<GPUTexturedMesh> GPUModelObjs,
                 glm::vec3 translationVec,
                 glm::vec3 rotationAxis,
                 float rotationAngle,
                 glm::vec3 scaleVec) : GPUModelObjs(std::move(GPUModelObjs)) {
        translationMatrix = glm::translate(translationMatrix, translationVec);
        rotationMatrix = glm::rotate(rotationMatrix, rotationAngle, rotationAxis);
        scaleMatrix = glm::scale(scaleMatrix, scaleVec);

        updateModelMatrix();
    }

    // No transforms
    RenderEntity(std::vector<GPUTexturedMesh> GPUModelObjs) : GPUModelObjs(std::move(GPUModelObjs)) {
        updateModelMatrix();
    }

    // Translation
    RenderEntity(std::vector<GPUTexturedMesh> GPUModelObjs, glm::vec3 translationVec) :
            RenderEntity(std::move(GPUModelObjs), translationVec, glm::vec3{1.0f}, 0.0, glm::vec3{1.0f}) {}

    // Rotation
    RenderEntity(std::vector<GPUTexturedMesh> GPUModelObjs, glm::vec3 rotationAxis, float rotationAngle) :
            RenderEntity(std::move(GPUModelObjs), glm::vec3{}, rotationAxis, rotationAngle, glm::vec3{}) {}

    // Translation & Scale
    RenderEntity(std::vector<GPUTexturedMesh> GPUModelObjs, glm::vec3 translationVec, glm::vec3 scaleVec) :
            RenderEntity(std::move(GPUModelObjs), translationVec, glm::vec3{}, 0.0, scaleVec) {}

    [[nodiscard]] glm::mat4 getModelMatrix() const {
        return modelMatrix;
    }

    [[nodiscard]] const std::vector<GPUTexturedMesh>& getGPUObjects() const {
        return GPUModelObjs;
    }
};


#endif //OPENGL_RENDERENTITY_H
