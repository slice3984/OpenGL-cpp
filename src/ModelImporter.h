//
// Created by Sebastian on 18.02.2024.
//

#ifndef OPENGL_MODELIMPORTER_H
#define OPENGL_MODELIMPORTER_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include "glm/glm.hpp"
#include "stb_image.h"
#include "util.h"
#include "ModelObject.h"

/*
 * - Only supports models without multiple materials per object
 * - MTL file has to include a map_Kd pointing to the texture
 */

class ModelImporter {
public:
    ModelImporter();
    std::vector<ModelObject> loadModel(const std::string& path);
    std::vector<std::vector<ModelObject>> loadModelFolder(const std::string& path);

private:
    [[nodiscard]] std::map<std::string, ModelObjectTexture> parseMtl(const std::string& path) const;
};


#endif //OPENGL_MODELIMPORTER_H
