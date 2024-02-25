//
// Created by Sebastian on 19.02.2024.
//

#ifndef OPENGL_MODELDATA_H
#define OPENGL_MODELDATA_H


#include "TexturedMeshData.h"

struct ModelData {
    std::string name;
    std::vector<TexturedMeshData> objects;
};

#endif //OPENGL_MODELDATA_H
