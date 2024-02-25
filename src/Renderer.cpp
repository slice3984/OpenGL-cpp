//
// Created by Sebastian on 23.02.2024.
//

#include "Renderer.h"


Renderer::Renderer(const Shader& shader) : shader(shader)  {}

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

        glActiveTexture(GL_TEXTURE0);

        for (const GPUTexturedMesh& obj : entity.getGPUObjects()) {
            glBindTexture(GL_TEXTURE_2D, obj.textureId);
            glBindVertexArray(obj.vao);

            glDrawArrays(GL_TRIANGLES, 0, obj.vertexCount);
            glBindVertexArray(0);
        }
    }
}


