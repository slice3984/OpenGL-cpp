//
// Created by Sebastian on 19.02.2024.
//

#ifndef OPENGL_OBJMODEL_H
#define OPENGL_OBJMODEL_H


#include "ModelObject.h"

struct ObjModel {
    std::string name;
    std::vector<ModelObject> objects;
};

#endif //OPENGL_OBJMODEL_H
