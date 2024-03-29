//
// Created by Sebastian on 18.02.2024.
//

#ifndef OPENGL_TEXTUREDMESHDATA_H
#define OPENGL_TEXTUREDMESHDATA_H
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "stb_image.h"
#include "ImageData.h"

class TexturedMeshData {
private:
    std::string name;
    ImageData texture{};
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;

public:
    TexturedMeshData(std::string&& name,
                     ImageData texture,
                     std::vector<glm::vec3>&& vertices,
                     std::vector<glm::vec2>&& texCoords
                ) :
            name{std::forward<std::string>(name)},
            texture(texture),
            vertices{std::forward<std::vector<glm::vec3>>(vertices)},
            texCoords{std::forward<std::vector<glm::vec2>>(texCoords)} {}

    TexturedMeshData(TexturedMeshData&& other) noexcept {
        name = other.name;
        texture = other.texture;
        vertices = other.vertices;
        texCoords = other.texCoords;

        other.texture.imageData = nullptr;
    }

    [[nodiscard]] const std::string& getName() const {
        return name;
    }

    [[nodiscard]] const ImageData& getImageData() const {
        return texture;
    }

    [[nodiscard]] const std::vector<glm::vec3>& getVertices() const {
        return vertices;
    }

    [[nodiscard]] const std::vector<glm::vec2>& getTexCoords() const {
        return texCoords;
    }

    ~TexturedMeshData() {
        // The same texture could be shared across multiple model objects and could be freed previously
        if (texture.imageData != nullptr) {
            stbi_image_free(texture.imageData);
            texture.imageData = nullptr;
        }
    }
};


#endif //OPENGL_TEXTUREDMESHDATA_H
