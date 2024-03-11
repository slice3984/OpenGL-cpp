//
// Created by Sebastian on 23.02.2024.
//

#include "Renderer.h"


Renderer::Renderer(const Shader& shader) : shader(shader)  {
    // Generate default textures
    // baseColor
    ImageData baseColor = util::loadImage("../assets/textures/default/baseColor.png");
    glActiveTexture(GL_TEXTURE0); // Unit 0 - BaseColor
    glGenTextures(1, &texBaseColor);
    glBindTexture(GL_TEXTURE_2D, texBaseColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, baseColor.width, baseColor.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, baseColor.imageData);
}

void Renderer::addQueue(const RenderQueue &renderQueue) {
    renderQueues[renderQueue.getName()] = &renderQueue;
}

bool Renderer::renderQueue(const std::string &queueName) const {
    if (auto it = renderQueues.find(queueName); it != renderQueues.end()) {
        shader.use();
        renderQueue(*it->second);
        return true;
    }

    return false;
}

bool Renderer::renderAllQueues() const {
    if (renderQueues.empty()) {
        return false;
    }

    shader.use();
    for (auto const& queue : renderQueues) {
        renderQueue(*queue.second);
    }

    return true;
}

void Renderer::renderQueue(const RenderQueue &queue) const {
    const std::vector<RenderEntity>& entities = queue.getQueue();

    for (const RenderEntity& entity : entities) {
        const glm::mat4 modelMatrix = entity.getModelMatrix();
        shader.setMat4("model", modelMatrix);

        for (const MeshPrimitive& primitive : entity.getModel().primitives) {
            // BaseColor
            MeshTexture* baseColorTex = primitive.material->baseColor;
            glActiveTexture(GL_TEXTURE0);

            shader.setVec4("factorsBaseColor", baseColorTex->factor);

            // Default tex
            if (!primitive.material->baseColor->textureStored) {
                glBindTexture(GL_TEXTURE_2D, texBaseColor);
            } else {
                glBindTexture(GL_TEXTURE_2D, baseColorTex->id);
            }

            glBindVertexArray(primitive.vao);
            glDrawElements(primitive.mode, primitive.count, primitive.dataType, 0);
            glBindVertexArray(0);
        }

        glActiveTexture(GL_TEXTURE0);
    }
}


