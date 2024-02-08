//
// Created by Sebastian on 07.02.2024.
//

#include "utils.h"

namespace utils {
    GLuint compileShader(std::string const &path, GLenum shaderType) {
        std::string shaderCode;

        try {
            std::ifstream shaderFile(path);

            if (shaderFile.is_open()) {
                std::string line;

                while (std::getline(shaderFile, line)) {
                    shaderCode += line + "\n";
                }

                shaderFile.close();
            } else {
                std::cout << "Unable to open the shader file\n";
            }
        } catch (const std::exception &e) {
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

    GLuint createProgram(GLuint vertShader, GLuint fragShader) {
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
}

