//
// Created by Sebastian on 04.03.2024.
//

#ifndef OPENGL_TEXTURESTORE_H
#define OPENGL_TEXTURESTORE_H
#include <unordered_map>
#include <string>
#include "glad/glad.h"
#include "MeshTypes.h"
#include "ImageData.h"

class TextureStore {
private:
    std::unordered_map<std::string, MeshTexture> textures;

public:
    MeshTexture* uploadTexture(const std::string& name, MeshTexture::TextureType type,
                               const ImageData& imageData, glm::vec4 factors = {1.0f, 1.0f, 1.0f, 1.0f});
    MeshTexture* addTexture(const std::string& name, MeshTexture texture);
    MeshTexture* getTexture(const std::string& name);
    bool hasTexture(const std::string& name);
};


#endif //OPENGL_TEXTURESTORE_H
