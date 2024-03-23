//
// Created by Sebastian on 03.03.2024.
//

#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H

#include <vector>
#include "MeshTypes.h"

struct Model {
    std::string name;
    std::vector<MeshPrimitive> primitives;
};

#endif //OPENGL_MODEL_H
