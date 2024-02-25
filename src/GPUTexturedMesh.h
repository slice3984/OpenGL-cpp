//
// Created by Sebastian on 23.02.2024.
//

#ifndef OPENGL_GPUTEXTUREDMESH_H
#define OPENGL_GPUTEXTUREDMESH_H

#include "glad/glad.h"

struct GPUTexturedMesh {
    GLuint textureId;
    GLuint vao;
    GLuint vbo;
    GLsizei vertexCount;
};

#endif //OPENGL_GPUTEXTUREDMESH_H
