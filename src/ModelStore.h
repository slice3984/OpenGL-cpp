//
// Created by Sebastian on 19.02.2024.
//

#ifndef OPENGL_MODELSTORE_H
#define OPENGL_MODELSTORE_H
#include <vector>
#include <map>
#include <optional>
#include <memory>
#include "glad/glad.h"
#include "TexturedMeshData.h"
#include "ModelData.h"
#include "GPUTexturedMesh.h"

class ModelStore {
public:
    bool registerModel(const ModelData& objModel);
    bool registerModel(const std::vector<ModelData>& objModels);
    [[nodiscard]] std::optional<std::reference_wrapper<const std::vector<GPUTexturedMesh>>> tryGetModel(const std::string& name) const;
    [[nodiscard]] const std::vector<GPUTexturedMesh>& getModel(const std::string& name) const;

private:
    std::map<std::string, std::vector<GPUTexturedMesh>> loadedModels;
};


#endif //OPENGL_MODELSTORE_H
