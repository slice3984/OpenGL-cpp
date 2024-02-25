//
// Created by Sebastian on 19.02.2024.
//

#include "ModelStore.h"
#include "GPUTexturedMesh.h"
#include "ImageData.h"

bool ModelStore::registerModel(const ModelData& objModel) {
    if (loadedModels.contains(objModel.name)) {
        return false;
    }

    std::vector<GPUTexturedMesh> objects;
    for (const TexturedMeshData& object : objModel.objects) {
        GPUTexturedMesh gpuModelObject{};
        gpuModelObject.vertexCount = object.getVertices().size();

        /*
         * Generate the final vertex attribute array
         * v.x v.y .v.z s t
         */

        // 3 for vertex coord - 2 for texture coord
        size_t bufferSize = object.getVertices().size() * 5;

        float vertexAttributes[bufferSize];

        for (size_t i = 0; i < object.getVertices().size();  i++) {
            size_t currBufferIdx = i * 5;

            glm::vec3 vertex = object.getVertices()[i];
            glm::vec2 texCoord = object.getTexCoords()[i];

            vertexAttributes[currBufferIdx] = vertex.x;
            vertexAttributes[currBufferIdx + 1] = vertex.y;
            vertexAttributes[currBufferIdx + 2] = vertex.z;
            vertexAttributes[currBufferIdx + 3] = texCoord.s;
            vertexAttributes[currBufferIdx + 4] = texCoord.t;
        }

        // Texture
        glGenTextures(1, &gpuModelObject.textureId);
        glBindTexture(GL_TEXTURE_2D, gpuModelObject.textureId);

        const ImageData& texture = object.getImageData();

        // RGB
        if (texture.nChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.imageData);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.imageData);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        // VAO
        glGenVertexArrays(1, &gpuModelObject.vao);
        glBindVertexArray(gpuModelObject.vao);

        // VBO
        glGenBuffers(1, &gpuModelObject.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, gpuModelObject.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAttributes), vertexAttributes, GL_STATIC_DRAW);

        // Attrib pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0); // Vert coords
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT))); // Tex coord
        glEnableVertexAttribArray(1);

        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        objects.push_back(gpuModelObject);
    }

    loadedModels[objModel.name] = objects;
    return true;
}

bool ModelStore::registerModel(const std::vector<ModelData> &objModels) {
    bool gotNewModel = false;

    for (const ModelData& model : objModels) {
        std::string modelName = model.name;

        if (!loadedModels.contains(modelName)) {
            gotNewModel = true;
            registerModel(model);
        }
    }

    return gotNewModel;
}

std::optional<std::reference_wrapper<const std::vector<GPUTexturedMesh>>> ModelStore::tryGetModel(const std::string &name) const {
    if (auto it = loadedModels.find(name); it != loadedModels.end()) {
        return { it->second };
    }

    return std::nullopt;
}

const std::vector<GPUTexturedMesh> &ModelStore::getModel(const std::string &name) const {
    return loadedModels.at(name);
}
