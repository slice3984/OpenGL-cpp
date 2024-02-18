//
// Created by Sebastian on 07.02.2024.
//

#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include "glad/glad.h"


namespace utils {
    GLuint compileShader(std::string const& path, GLenum shaderType);
    GLuint createProgram(GLuint vertShader, GLuint fragShader);
}

#endif //OPENGL_UTILS_H
