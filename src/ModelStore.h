//
// Created by Sebastian on 19.02.2024.
//

#ifndef OPENGL_MODELSTORE_H
#define OPENGL_MODELSTORE_H
#include <vector>
#include <map>
#include "glad/glad.h"
#include "ModelObject.h"
#include "ObjModel.h"

struct GPUModelObject {
    GLuint textureId;
    GLuint vao;
    GLuint vbo;
    GLsizei vertexCount;
};

class ModelStore {
public:
    bool registerModel(const ObjModel& objModel);
    bool registerModel(const std::vector<ObjModel>& objModels);
    void renderModel(const std::string& name);

private:
    std::map<std::string, std::vector<GPUModelObject>> loadedModels;
};


#endif //OPENGL_MODELSTORE_H
