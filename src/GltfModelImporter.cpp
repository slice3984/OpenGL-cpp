//
// Created by Sebastian on 06.03.2024.
//

#include "GltfModelImporter.h"

GltfModelImporter::GltfModelImporter(TextureStore &textureStore, MaterialStore &materialStore, ModelStore &modelStore) :
        textureStore(textureStore), materialStore(materialStore), modelStore(modelStore) {}

void GltfModelImporter::uploadModel(const std::string &file) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;

    std::string err, warn;
    loader.LoadASCIIFromFile(&model, &err, &warn, file);

    const tinygltf::Scene &scene = model.scenes[model.defaultScene];
    std::string modelName = util::extractFileName(file);

    std::vector<MeshPrimitive> primitives;
    tinygltf::Node &node = model.nodes[0];
    bindNodes(primitives, modelName, model, node);

    modelStore.addModel(modelName, {modelName, std::move(primitives)});
}

void GltfModelImporter::bindNodes(std::vector<MeshPrimitive> &primitives, const std::string &modelName,
                                  tinygltf::Model &model, tinygltf::Node &node) {
    // The node itself might have a mesh
    // Nodes store indices to the meshes in the model meshes buffer
    // Check if the node mesh id is in range of the model meshes buffer
    if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
        bindMesh(primitives, modelName, model, model.meshes[node.mesh]);
    }

    // Children of nodes
    for (size_t nodeChildrenIdx: node.children) {
        // Recursively walk the tree and bind the nodes
        bindNodes(primitives, modelName, model, model.nodes[nodeChildrenIdx]);
    }
}

void GltfModelImporter::bindMesh(std::vector<MeshPrimitive> &primitives, const std::string &modelName,
                                 tinygltf::Model &model, tinygltf::Mesh &mesh) {
    // Primitives contain actual meshes to be rendered
    // There can be multiple primitives in case a mesh uses different materials
    for (const tinygltf::Primitive &primitive: mesh.primitives) {
        MeshPrimitive meshPrimitive;
        meshPrimitive.mode = primitive.mode;

        glGenVertexArrays(1, &meshPrimitive.vao);
        glBindVertexArray(meshPrimitive.vao);

        for (auto &attrib: primitive.attributes) {
            tinygltf::Accessor &accessor = model.accessors[attrib.second];
            tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

            int vertLocation = -1;

            if (attrib.first == "POSITION") vertLocation = 0;
            else if (attrib.first == "NORMAL") vertLocation = 1;
            else if (attrib.first == "TEXCOORD_0") vertLocation = 2;

            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(bufferView.target, vbo);
            glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(bufferView.byteOffset),
                         GL_STATIC_DRAW);

            glVertexAttribPointer(vertLocation, accessor.type, accessor.componentType,
                                  accessor.normalized ? GL_TRUE : GL_FALSE, bufferView.byteStride,
                                  (void *) accessor.byteOffset);

            glEnableVertexAttribArray(vertLocation);
            glBindBuffer(bufferView.target, 0);
        }

        // Possible ebo
        assert(primitive.indices < model.accessors.size() || primitive.indices >= model.accessors.size());
        tinygltf::Accessor &accessorEbo = model.accessors[primitive.indices];
        tinygltf::BufferView &bufferView = model.bufferViews[accessorEbo.bufferView];
        tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(bufferView.target, ebo);
        glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(bufferView.byteOffset), GL_STATIC_DRAW);

        meshPrimitive.count = accessorEbo.count;
        meshPrimitive.dataType = accessorEbo.componentType;

        glBindVertexArray(0);

        // Material
        tinygltf::Material &material = model.materials[primitive.material];

        MeshMaterial meshMaterial{};
        std::string materialKeyName = generateMaterialKeyName(modelName, material.name);

        if (materialStore.hasMaterial(materialKeyName)) {
            meshPrimitive.material = materialStore.getMaterial(materialKeyName);
        } else {
            int textureIdx = -1;

            // BaseColor
            tinygltf::PbrMetallicRoughness &pbrMetallicRoughness = material.pbrMetallicRoughness;

            // Will be used for object color if no texture is given
            glm::vec4 factors{
                    pbrMetallicRoughness.baseColorFactor.at(0), // R
                    pbrMetallicRoughness.baseColorFactor.at(1), // G
                    pbrMetallicRoughness.baseColorFactor.at(2), // B
                    pbrMetallicRoughness.baseColorFactor.at(3)  // A
            };

            textureIdx = pbrMetallicRoughness.baseColorTexture.index;
            if (textureIdx >= 0) {
                meshMaterial.baseColor = getOrGenerateTexture(model, modelName, MeshTexture::BASE_COLOR, textureIdx, factors);
            } else {
                std::string keyName = generateTextureKeyName(modelName, MeshTexture::BASE_COLOR);
                meshMaterial.baseColor = textureStore.addTexture(keyName,{ false, MeshTexture::MeshTexture::BASE_COLOR, factors, 0 });
            }

            // MetallicRoughness
            factors = {
                    pbrMetallicRoughness.metallicFactor,
                    pbrMetallicRoughness.roughnessFactor,
                    0.0f,
                    0.0f
            };

            textureIdx = pbrMetallicRoughness.metallicRoughnessTexture.index;
            if (textureIdx >= 0) {
                meshMaterial.metallicRoughness = getOrGenerateTexture(model, modelName, MeshTexture::METALLIC_ROUGHNESS, textureIdx, factors);
            } else {
                std::string keyName = generateTextureKeyName(modelName, MeshTexture::METALLIC_ROUGHNESS);
                meshMaterial.metallicRoughness = textureStore.addTexture(keyName, { false, MeshTexture::MeshTexture::METALLIC_ROUGHNESS, factors, 0 });
            }

            // Normal
            factors = {material.normalTexture.scale, 0.0f, 0.0f, 0.0f};

            textureIdx = material.normalTexture.index;
            if (textureIdx >= 0) {
                meshMaterial.normal = getOrGenerateTexture(model, modelName, MeshTexture::NORMAL, textureIdx, factors);
            } else {
                std::string keyName = generateTextureKeyName(modelName, MeshTexture::NORMAL);
                meshMaterial.normal = textureStore.addTexture(keyName, { false, MeshTexture::MeshTexture::NORMAL, factors, 0 });
            }

            // Occlusion
            factors = {material.occlusionTexture.strength, 0.0f, 0.0f, 0.0f};

            textureIdx = material.occlusionTexture.index;
            if (textureIdx >= 0) {
                meshMaterial.occlusion = getOrGenerateTexture(model, modelName, MeshTexture::OCCLUSION, textureIdx, factors);
            } else {
                std::string keyName = generateTextureKeyName(modelName, MeshTexture::OCCLUSION);
                meshMaterial.occlusion = textureStore.addTexture(keyName, {false, MeshTexture::MeshTexture::OCCLUSION, factors, 0});
            }

            // Emissive
            factors = {
                    material.emissiveFactor[0],
                    material.emissiveFactor[1],
                    material.emissiveFactor[2],
                    0.0f
            };

            textureIdx = material.emissiveTexture.index;
            if (textureIdx >= 0) {
                meshMaterial.emissive = getOrGenerateTexture(model, modelName, MeshTexture::EMISSIVE, textureIdx, factors);
            } else {
                std::string keyName = generateTextureKeyName(modelName, MeshTexture::EMISSIVE);
                meshMaterial.emissive = textureStore.addTexture(keyName, { false, MeshTexture::MeshTexture::EMISSIVE, factors, 0 });
            }

            meshPrimitive.material = materialStore.addMaterial(materialKeyName, meshMaterial);
        }

        primitives.emplace_back(meshPrimitive);
    }
}

std::string GltfModelImporter::generateMaterialKeyName(const std::string &modelName, const std::string &name) {
    return modelName + "/" + name + "/material";
}

std::string GltfModelImporter::generateTextureKeyName(const std::string &modelName, MeshTexture::TextureType type) {
    std::string texType;

    switch (type) {
        case MeshTexture::BASE_COLOR:
            texType = "baseColor";
            break;
        case MeshTexture::METALLIC_ROUGHNESS:
            texType = "metallicRoughness";
            break;
        case MeshTexture::NORMAL:
            texType = "normal";
            break;
        case MeshTexture::OCCLUSION:
            texType = "occlusion";
            break;
        case MeshTexture::EMISSIVE:
            texType = "emissive";
            break;
    }

    return modelName + "/" + texType + "/texture";
}

MeshTexture *GltfModelImporter::getOrGenerateTexture(tinygltf::Model &model, const std::string &modelName,
                                                     MeshTexture::TextureType type, size_t textureIdx,
                                                     glm::vec4 factors) {
    std::string mapKey = generateTextureKeyName(modelName, type);

    if (textureStore.hasTexture(mapKey)) {
        return textureStore.getTexture(mapKey);
    }

    tinygltf::Texture &modelTex = model.textures[textureIdx];
    tinygltf::Image &image = model.images[modelTex.source];

    ImageData imageData = {true, image.image.data(), image.width, image.height, image.component};

    return textureStore.uploadTexture(mapKey, type, imageData, factors);
}

void GltfModelImporter::uploadModelDirectory(const std::string &path) {
    std::filesystem::recursive_directory_iterator iter;

    try {
        iter = std::filesystem::recursive_directory_iterator(path);
    } catch (...) {
        throw;
    }

    // Accept other gltf files in future
    for (const auto& entry : iter) {
        if (entry.path().extension() == ".gltf") {
            uploadModel(util::normalizePath(entry.path()));
        }
    }
}
