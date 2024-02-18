//
// Created by Sebastian on 18.02.2024.
//

#ifndef OPENGL_MODELIMPORTER_H
#define OPENGL_MODELIMPORTER_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include "glm/glm.hpp"
#include "stb_image.h"
#include "util.h"
#include "ModelObject.h"


class ModelImporter {
public:
    ModelImporter();
    std::vector<ModelObject> loadModel(const std::string& path);

private:
    [[nodiscard]] std::map<std::string, ModelObjectTexture> parseMtl(const std::string& path) const;
};


#endif //OPENGL_MODELIMPORTER_H
