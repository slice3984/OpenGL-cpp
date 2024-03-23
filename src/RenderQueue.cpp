//
// Created by Sebastian on 23.02.2024.
//

#include "RenderQueue.h"

#include <utility>

RenderQueue &RenderQueue::addModel(const RenderEntity& renderEntity) {
    queue.push_back(renderEntity);
    return *this;
}

RenderQueue::RenderQueue(std::string name) : name(std::move(name)) {}

const std::string& RenderQueue::getName() const {
    return name;
}

const std::vector<RenderEntity>& RenderQueue::getQueue() const {
    return queue;
}

std::vector<RenderEntity>& RenderQueue::getQueue() {
    return queue;
}
