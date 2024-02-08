//
// Created by Sebastian on 08.02.2024.
//

#include "Shader.h"

Shader::Shader(const std::string &vertPath, const std::string &fragPath) {
    GLuint vertShader = compileShader(vertPath, GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragPath, GL_FRAGMENT_SHADER);

    progId = compileProgram(vertShader, fragShader);
}

void Shader::use() const {
    glUseProgram(progId);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(progId, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(progId, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(progId, name.c_str()), value);
}

GLuint Shader::compileShader(const std::string &shaderPath, GLenum shaderType) const {
    std::string shaderCode;

    try {
        std::ifstream shaderFile(shaderPath);

        if (shaderFile.is_open()) {
            std::string line;

            while (std::getline(shaderFile, line)) {
                shaderCode += line + "\n";
            }

            shaderFile.close();
        } else {
            std::cout << "Unable to open the shader file" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << std::format("Error: {}", e.what());
    }

    GLuint shader = glCreateShader(shaderType);

    const char *c_str = shaderCode.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    GLint success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        std::cout << std::format("Shader complication failed: {}", infoLog);
    }

    return shader;
}

GLuint Shader::compileProgram(GLuint vertShader, GLuint fragShader) {
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    GLint success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << std::format("Shader program linkage error: {}", infoLog);
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}
