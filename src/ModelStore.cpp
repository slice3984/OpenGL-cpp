//
// Created by Sebastian on 04.03.2024.
//

#include "ModelStore.h"

Model& ModelStore::addModel(const std::string &name, Model model) {
    if (models.contains(name)) {
        return models[name];
    }

    models[name] = std::move(model);
    return models[name];
}

Model &ModelStore::getModel(const std::string &name) {
    return models[name];
}
