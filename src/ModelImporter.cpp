//
// Created by Sebastian on 18.02.2024.
//

#include "ModelImporter.h"

ModelImporter::ModelImporter() = default;

std::vector<ModelObject> ModelImporter::loadModel(const std::string &path) {
    std::vector<ModelObject> modelObjects;
    std::map<std::string, ModelObjectTexture> textures;
    std::fstream objFile(path);

    // Object properties
    std::string name;
    std::string material;
    std::vector<glm::vec3> vertices;
    std::vector<size_t> vertexIndices;
    std::vector<glm::vec2> texCoords;
    std::vector<size_t> texCoordIndices;

    std::vector<glm::vec3> finalVertices;
    std::vector<glm::vec2> finalTexCoords;

    if (objFile.is_open()) {

        std::string line;
        while (std::getline(objFile, line)) {
            std::istringstream iss(line);

            std::string type;
            iss >> type;

            // Parse mtl file
            if (type == "mtllib") {
                std::string texturePath;
                iss >> texturePath;

                textures = parseMtl(texturePath);
            }

            // Object name
            if (type == "o") {
                // Store previously parsed object
                if (!name.empty()) {
                    modelObjects.emplace_back(
                            std::move(name),
                            textures[material],
                            std::move(finalVertices),
                            std::move(finalTexCoords)
                    );

                    finalVertices.clear();
                    finalTexCoords.clear();
                }

                iss >> name;
            }

            // Vertex
            if (type == "v") {
                glm::vec3 vertex;

                std::string x, y, z;
                iss >> x >> y >> z;

                vertex.x = std::stof(x);
                vertex.y = std::stof(y);
                vertex.z = std::stof(z);

                vertices.push_back(vertex);
            }

            // Texture coord
            if (type == "vt") {
                glm::vec2 textureCoord;

                std::string s, t;
                iss >> s >> t;

                textureCoord.s = std::stof(s);
                textureCoord.t = std::stof(t);

                texCoords.push_back(textureCoord);
            }

            // Material
            if (type == "usemtl") {
                iss >> material;
            }

            // Faces
            if (type == "f") {
                std::string vertTexCoordIndexPair;

                for (size_t i = 0; i < 3; i++) {
                    iss >> vertTexCoordIndexPair;
                    std::vector<std::string> pair = util::splitString(vertTexCoordIndexPair, '/');

                    // Indices start with 1
                    glm::vec3 vertex = vertices[std::stoi(pair[0]) - 1];
                    glm::vec2 texCoord = texCoords[std::stoi(pair[1]) - 1];

                    finalVertices.push_back(vertex);
                    finalTexCoords.push_back(texCoord);
                }
            }
        }

        // Last Object
        if (!name.empty()) {
            modelObjects.emplace_back(
                    std::move(name),
                    textures[material],
                    std::move(finalVertices),
                    std::move(finalTexCoords)
            );
        }
    }

    objFile.close();

    return modelObjects;
}

std::map<std::string, ModelObjectTexture> ModelImporter::parseMtl(const std::string &path) const {
    std::map<std::string, ModelObjectTexture> textures;
    std::vector<std::pair<std::string, std::string>> materials;

    std::fstream mtlFile(path);

    std::string line;
    std::string materialName;
    std::string materialPath;

    while (std::getline(mtlFile, line)) {
        std::istringstream iss(line);

        std::string type;
        iss >> type;

        // Material name
        if (type == "newmtl") {
            iss >> materialName;
        }

        // Texture path
        if (type == "map_Kd") {
            iss >> materialPath;

            materials.emplace_back(materialName, materialPath);
        }
    }

    mtlFile.close();

    for (const auto &mat: materials) {
        int width{}, height{}, nChannels{};
        unsigned char *data = stbi_load(mat.second.c_str(), &width, &height, &nChannels, 0);

        textures[mat.first] = ModelObjectTexture{data, width, height, nChannels};
    }

    return textures;
}


