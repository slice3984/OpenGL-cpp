//
// Created by Sebastian on 03.03.2024.
//

#ifndef OPENGL_MESHTYPES_H
#define OPENGL_MESHTYPES_H
#include "glm/glm.hpp"
#include "glad/glad.h"

struct MeshTexture {
    enum TextureType {
        BASE_COLOR, METALLIC_ROUGHNESS, NORMAL, OCCLUSION, EMISSIVE
    };

    bool textureStored;
    TextureType type{};
    glm::vec4 factor{};
    GLuint id{};
};

struct MeshMaterial {
    MeshTexture* baseColor{nullptr};
    MeshTexture* metallicRoughness{nullptr};
    MeshTexture* normal{nullptr};
    MeshTexture* occlusion{nullptr};
    MeshTexture* emissive{nullptr};
};

struct MeshPrimitive {
    GLenum mode{}; // Kind of primitive to render (Line / Triangle..)
    GLenum dataType{};
    GLuint vao{};
    GLsizei count{};
    MeshMaterial* material{nullptr};
};

#endif //OPENGL_MESHTYPES_H
