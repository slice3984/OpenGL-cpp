//
// Created by Sebastian on 08.02.2024.
//

#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <iostream>
#include <string>
#include <format>
#include <fstream>
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"


class Shader {
public:
    GLuint progId;

    Shader(const std::string& vertPath, const std::string& fragPath);
    void use() const;

    // Utility
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& matrix) const;
    void setVec4(const std::string& name, const glm::vec4& vector) const;

private:
    GLuint compileShader(const std::string& shaderPath, GLenum shaderType) const;
    GLuint compileProgram(GLuint vertShader, GLuint fragShader);
};


#endif //OPENGL_SHADER_H
