//
// Created by Sebastian on 06.03.2024.
//

#ifndef OPENGL_GLTFMODELIMPORTER_H
#define OPENGL_GLTFMODELIMPORTER_H
#include <unordered_map>
#include <string>
#include <cassert>
#include "tiny_gltf.h"
#include "MeshTypes.h"
#include "Model.h"
#include "TextureStore.h"
#include "MaterialStore.h"
#include "ModelStore.h"
#include "util.h"

class GltfModelImporter {
private:
    TextureStore& textureStore;
    MaterialStore& materialStore;
    ModelStore& modelStore;

    void bindNodes(std::vector<MeshPrimitive>& primitives, const std::string& modelName, tinygltf::Model &model, tinygltf::Node &node);
    void bindMesh(std::vector<MeshPrimitive>& primitives, const std::string& modelName, tinygltf::Model& model, tinygltf::Mesh& mesh);
    static std::string generateMaterialKeyName(const std::string& modelName, const std::string& name);
    static std::string generateTextureKeyName(const std::string& modelName, MeshTexture::TextureType type);
    MeshTexture* getOrGenerateTexture(tinygltf::Model& model, const std::string& modelName, MeshTexture::TextureType type, size_t textureIdx, glm::vec4 factors);

public:
    GltfModelImporter(TextureStore& textureStore, MaterialStore& materialStore, ModelStore& modelStore);
    void uploadModel(const std::string& file);
    void uploadModelDirectory(const std::string& path);
};


#endif //OPENGL_GLTFMODELIMPORTER_H
