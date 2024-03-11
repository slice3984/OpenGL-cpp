//
// Created by Sebastian on 04.03.2024.
//

#include "MaterialStore.h"

MeshMaterial *MaterialStore::addMaterial(const std::string &name, MeshMaterial material) {
    if (materials.contains(name)) {
        return nullptr;
    }

    materials[name] = material;
    return &materials[name];
}

MeshMaterial *MaterialStore::getMaterial(const std::string &name) {
    if (!materials.contains(name)) {
        return nullptr;
    }

    return &materials[name];
}

bool MaterialStore::hasMaterial(const std::string &name) const {
    return materials.contains(name);
}
