//
// Created by Sebastian on 19.02.2024.
//

#include "ModelStore.h"

bool ModelStore::registerModel(const ObjModel& objModel) {
    if (loadedModels.contains(objModel.name)) {
        return false;
    }

    std::vector<GPUModelObject> objects;
    for (const ModelObject& object : objModel.objects) {
        GPUModelObject gpuModelObject{};
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

        const ModelObjectTexture& texture = object.getImageData();

        // RGB
        if (texture.nChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.textureData);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.textureData);
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

void ModelStore::renderModel(const std::string &name) {
    if (!loadedModels.contains(name)) {
        return;
    }

    glActiveTexture(GL_TEXTURE0);

    for (const GPUModelObject& object : loadedModels[name]) {
        glBindTexture(GL_TEXTURE_2D, object.textureId);
        glBindVertexArray(object.vao);

        glDrawArrays(GL_TRIANGLES, 0, object.vertexCount);
        glBindVertexArray(0);
    }
}

bool ModelStore::registerModel(const std::vector<ObjModel> &objModels) {
    bool gotNewModel = false;

    for (const ObjModel& model : objModels) {
        std::string modelName = model.name;

        if (!loadedModels.contains(modelName)) {
            gotNewModel = true;
            registerModel(model);
        }
    }

    return gotNewModel;
}
