//
// Created by Sebastian on 23.02.2024.
//

#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H
#include <map>
#include "RenderQueue.h"
#include "MeshTypes.h"
#include "Shader.h"
#include "util.h"

class Renderer {
private:
    // Default textures
    GLuint texBaseColor;

    const Shader& shader;
    std::map<std::string, const RenderQueue*> renderQueues{};
    void renderQueue(const RenderQueue& queue) const;

public:
    explicit Renderer(const Shader& shader);
    void addQueue(const RenderQueue& renderQueue);
    bool renderQueue(const std::string& queueName) const;
    bool renderAllQueues() const;
};


#endif //OPENGL_RENDERER_H
