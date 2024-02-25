//
// Created by Sebastian on 18.02.2024.
//

#include "ModelImporter.h"

ModelImporter::ModelImporter() = default;

ModelData ModelImporter::loadModel(const std::string &path) {
    std::vector<TexturedMeshData> modelObjects;
    std::map<std::string, ImageData> textures;
    std::fstream objFile(path);

    // Object properties
    std::string name;
    std::string objectName;
    std::string material;
    std::vector<glm::vec3> vertices;
    std::vector<size_t> vertexIndices;
    std::vector<glm::vec2> texCoords;
    std::vector<size_t> texCoordIndices;

    std::vector<glm::vec3> finalVertices;
    std::vector<glm::vec2> finalTexCoords;

    if (objFile.is_open()) {
        name = util::extractFileName(path);

        std::string line;
        while (std::getline(objFile, line)) {
            std::istringstream iss(line);

            std::string type;
            iss >> type;

            // Parse mtl file
            if (type == "mtllib") {
                std::string texturePath;
                iss >> texturePath;

                // We expect the MTL file to be in the same directory
                textures = parseMtl(util::stripLastWordFromPath(path) + texturePath);
            }

            // Object name
            if (type == "o") {
                // Store previously parsed object
                if (!objectName.empty()) {
                    modelObjects.emplace_back(
                            std::move(objectName),
                            textures[material],
                            std::move(finalVertices),
                            std::move(finalTexCoords)
                    );

                    finalVertices.clear();
                    finalTexCoords.clear();
                }

                iss >> objectName;
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
        if (!objectName.empty()) {
            modelObjects.emplace_back(
                    std::move(objectName),
                    textures[material],
                    std::move(finalVertices),
                    std::move(finalTexCoords)
            );
        }
    }

    objFile.close();

    return {name, std::move(modelObjects)};
}

std::map<std::string, ImageData> ModelImporter::parseMtl(const std::string &path) const {
    std::map<std::string, ImageData> textures;
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
            std::string materialFileName;
            iss >> materialFileName;

            materialPath = util::stripLastWordFromPath(path) + materialFileName;
            materials.emplace_back(materialName, materialPath);
        }
    }

    mtlFile.close();

    for (const auto &mat: materials) {
        textures[mat.first] = util::loadImage(mat.second);
    }

    return textures;
}

std::vector<ModelData> ModelImporter::loadModelFolder(const std::string &path) {
    std::vector<ModelData> modelObjects;

    // Recursive iterate directories
    std::filesystem::recursive_directory_iterator iter;

    try {
        iter = std::filesystem::recursive_directory_iterator(path);
    } catch (...) {
        throw;
    }

    for (const auto& entry : iter) {
        if (std::filesystem::is_regular_file(entry)) {
            // Check if obj file
            if (entry.path().extension() == ".obj") {
                modelObjects.emplace_back(loadModel(util::normalizePath(entry.path())));
            }
        }
    }

    return modelObjects;
}


