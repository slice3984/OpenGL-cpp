//
// Created by Sebastian on 23.02.2024.
//

#ifndef OPENGL_RENDERENTITY_H
#define OPENGL_RENDERENTITY_H

#include <memory>
#include <utility>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model.h"

class RenderEntity {
private:
    Model& model;
    glm::mat4 m_modelMatrix = glm::identity<glm::mat4>();
    glm::mat4 m_translationMatrix = glm::identity<glm::mat4>();
    glm::mat4 m_rotationMatrix = glm::identity<glm::mat4>();
    glm::mat4 m_scaleMatrix = glm::identity<glm::mat4>();

    glm::vec3 m_translationVec{};
    glm::vec3 m_rotationVec{};

private:
    float m_rotationAngle;
    glm::vec3 m_scaleVec{};

    void updateModelMatrix() {
        m_modelMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
    }

public:
    // Constructors
    RenderEntity(Model& model,
                 glm::vec3 translationVec,
                 glm::vec3 rotationAxis,
                 float rotationAngle,
                 glm::vec3 scaleVec) : model(model) {
        m_translationVec = translationVec;
        m_rotationVec = rotationAxis;
        m_rotationAngle = rotationAngle;
        m_scaleVec = scaleVec;

        recalculateTransformationMatrices(false);
        updateModelMatrix();
    }

    // No transforms
    RenderEntity(Model& model) :
            RenderEntity(model, glm::vec3{}, glm::vec3{0.0f, 1.0f, 0.0f}, 0.0, glm::vec3{1.0f})  {}

    // Translation
    RenderEntity(Model& model, glm::vec3 translationVec) :
            RenderEntity(model, translationVec, glm::vec3{1.0f}, 0.0, glm::vec3{1.0f}) {}

    // Rotation
    RenderEntity(Model& model, glm::vec3 rotationAxis, float rotationAngle) :
            RenderEntity(model, glm::vec3{}, rotationAxis, rotationAngle, glm::vec3{1.0f}) {}

    // Translation & Scale
    RenderEntity(Model& model, glm::vec3 translationVec, glm::vec3 scaleVec) :
            RenderEntity(model, translationVec, glm::vec3{0.0f, 1.0f, 0.0f}, 0.0, scaleVec) {}

    void recalculateTransformationMatrices(bool reset) {
        if (reset) {
            m_translationMatrix = glm::identity<glm::mat4>();
            m_rotationMatrix = glm::identity<glm::mat4>();
            m_scaleMatrix = glm::identity<glm::mat4>();
        }

        m_translationMatrix = glm::translate(m_translationMatrix, m_translationVec);
        m_rotationMatrix = glm::rotate(m_rotationMatrix, m_rotationAngle, m_rotationVec);
        m_scaleMatrix = glm::scale(m_scaleMatrix, m_scaleVec);
    }

    void recalculateTransformsAndMatrices(bool reset) {
        recalculateTransformationMatrices(reset);
        updateModelMatrix();
    }

    [[nodiscard]] glm::mat4 getModelMatrix() const {
        return m_modelMatrix;
    }

    glm::mat4& getModelMatrix() {
        return m_modelMatrix;
    }

    [[nodiscard]] const Model& getModel() const {
        return model;
    }

    [[nodiscard]] const glm::vec3& getTranslationVec() const {
        return m_translationVec;
    }

    void setTranslationVec(const glm::vec3& mTranslationVec) {
        m_translationVec = mTranslationVec;
    }

    [[nodiscard]] const glm::vec3& getRotationVec() const {
        return m_rotationVec;
    }

    void setRotationVec(const glm::vec3 &mRotationVec) {
        m_rotationVec = mRotationVec;
    }

    [[nodiscard]] float getRotationAngle() const {
        return glm::degrees(m_rotationAngle);
    }

    void setRotationAngle(float mRotationAngle) {
        m_rotationAngle = glm::radians(mRotationAngle);
    }

    [[nodiscard]] const glm::vec3& getScaleVec() const {
        return m_scaleVec;
    }

    void setScaleVec(const glm::vec3 &mScaleVec) {
        m_scaleVec = mScaleVec;
    }
};


#endif //OPENGL_RENDERENTITY_H
