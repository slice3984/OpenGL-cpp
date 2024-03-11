//
// Created by Sebastian on 04.03.2024.
//

#ifndef OPENGL_MATERIALSTORE_H
#define OPENGL_MATERIALSTORE_H
#include <unordered_map>
#include <string>
#include "MeshTypes.h"

class MaterialStore {
private:
    std::unordered_map<std::string, MeshMaterial> materials;

public:
    MeshMaterial* addMaterial(const std::string& name, MeshMaterial material);
    MeshMaterial* getMaterial(const std::string& name);
    bool hasMaterial(const std::string& name) const;
};


#endif //OPENGL_MATERIALSTORE_H
