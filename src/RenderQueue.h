//
// Created by Sebastian on 23.02.2024.
//

#ifndef OPENGL_RENDERQUEUE_H
#define OPENGL_RENDERQUEUE_H
#include <vector>
#include <string>
#include "RenderEntity.h"

class RenderQueue {
private:
    std::string name;
    std::vector<RenderEntity> queue;

public:
    explicit RenderQueue(std::string name);
    RenderQueue& addModel(const RenderEntity& renderEntity);
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const std::vector<RenderEntity>& getQueue() const;
};


#endif //OPENGL_RENDERQUEUE_H
