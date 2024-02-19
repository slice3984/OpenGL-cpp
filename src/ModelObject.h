//
// Created by Sebastian on 18.02.2024.
//

#ifndef OPENGL_MODELOBJECT_H
#define OPENGL_MODELOBJECT_H
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "glm/glm.hpp"

struct ModelObjectTexture {
    unsigned char* textureData;
    int width;
    int height;
    int nChannels;
};

class ModelObject {
private:
    std::string name;
    ModelObjectTexture texture{};
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;

public:
    ModelObject(std::string&&  name,
                ModelObjectTexture texture,
                std::vector<glm::vec3>&& vertices,
                std::vector<glm::vec2>&& texCoords
                ) :
            name{std::forward<std::string>(name)},
            texture(texture),
            vertices{std::forward<std::vector<glm::vec3>>(vertices)},
            texCoords{std::forward<std::vector<glm::vec2>>(texCoords)} {}

    ModelObject(ModelObject&& other) noexcept {
        name = other.name;
        texture = other.texture;
        vertices = other.vertices;
        texCoords = other.texCoords;

        other.texture.textureData = nullptr;
    }

    [[nodiscard]] const std::string& getName() const {
        return name;
    }

    [[nodiscard]] const ModelObjectTexture& getImageData() const {
        return texture;
    }

    [[nodiscard]] const std::vector<glm::vec3>& getVertices() const {
        return vertices;
    }

    [[nodiscard]] const std::vector<glm::vec2>& getTexCoords() const {
        return texCoords;
    }

    ~ModelObject() {
        // The same texture could be shared across multiple model objects and could be freed previously
        if (texture.textureData != nullptr) {
            stbi_image_free(texture.textureData);
            texture.textureData = nullptr;
        }
    }
};


#endif //OPENGL_MODELOBJECT_H
