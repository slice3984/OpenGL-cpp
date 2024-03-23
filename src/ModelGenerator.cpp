//
// Created by Sebastian on 11.03.2024.
//

#include "ModelGenerator.h"

namespace ModelGenerator {

    void generateFloor(const std::string &modelName, float height, float width, ImageData tileTexture, float anisotropicFiltering,
                       bool centerFloor, TextureStore &textureStore, MaterialStore &materialStore, ModelStore &modelStore) {
        // Half sizes
        float hw = width / 2;
        float hh = height / 2;

        GLfloat vertexAttributes[]{
                // Vertices                                                  // Normals        // Texture Coords
                centerFloor ? -hw : 0.0f, 0.0f, centerFloor ? hh : 0.0f,     0.0f, 1.0f, 0.0f, 0.0f, 0.0f,            // Bottom Left
                centerFloor ? hw : width, 0.0f, centerFloor ? hh : 0.0f,     0.0f, 1.0f, 0.0f, width / 4, 0.0f,       // Bottom Right
                centerFloor ? hw : width, 0.0f, centerFloor ? -hh : -height, 0.0f, 1.0f, 0.0f, width / 4, height / 4, // Top Right
                centerFloor ? -hw : 0.0f, 0.0f, centerFloor ? -hh : -height, 0.0f, 1.0f, 0.0f, 0.0f, height / 4       // Top Left
        };

        GLushort indices[] = {
                0, 1, 2,
                0, 2, 3
        };

        // Texture
        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicFiltering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (tileTexture.nChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tileTexture.width, tileTexture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tileTexture.imageData);

        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tileTexture.width, tileTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tileTexture.imageData);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        GLuint vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        // VAO
        glBindVertexArray(vao);

        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAttributes), vertexAttributes, GL_STATIC_DRAW);

        // EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Attrib pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(2);

        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);



        MeshTexture* texture = textureStore.addTexture("floorTile", { true, MeshTexture::BASE_COLOR, {1.0f, 1.0f, 1.0f, 1.0f}, textureId});

        MeshMaterial* material = materialStore.addMaterial("floorTileMaterial", { texture, nullptr, nullptr, nullptr, nullptr });
        MeshPrimitive meshPrimitive = { GL_TRIANGLES, GL_UNSIGNED_SHORT, vao, 6, material};

        modelStore.addModel(modelName, { modelName, std::vector<MeshPrimitive> { meshPrimitive }});
    }
}