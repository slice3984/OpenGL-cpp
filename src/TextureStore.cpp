//
// Created by Sebastian on 04.03.2024.
//

#include "TextureStore.h"

MeshTexture* TextureStore::uploadTexture(const std::string &name, MeshTexture::TextureType type,
                                 const ImageData &imageData, glm::vec4 factors) {
    if (textures.contains(name)) {
        return nullptr;
    }

    MeshTexture texture{};
    texture.textureStored = true;
    texture.type = type;
    texture.factor = factors;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format = GL_RGBA;

    switch (imageData.nChannels) {
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData.width, imageData.height, 0, format, GL_UNSIGNED_BYTE, imageData.imageData);

    auto it = textures.insert(std::make_pair(name, texture));
    return std::addressof(it.first->second);
}

MeshTexture* TextureStore::addTexture(const std::string &name, MeshTexture texture) {
    if (textures.contains(name)) {
        return nullptr;
    }

    textures[name] = texture;
    return &textures[name];
}

MeshTexture* TextureStore::getTexture(const std::string &name) {
    if (!textures.contains(name)) {
        return nullptr;
    }

    return &textures[name];
}

bool TextureStore::hasTexture(const std::string &name) {
    return textures.contains(name);
}
