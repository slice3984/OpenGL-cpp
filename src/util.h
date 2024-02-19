//
// Created by Sebastian on 16.02.2024.
//

#ifndef OPENGL_UTIL_H
#define OPENGL_UTIL_H
#include <filesystem>
#include <iostream>
#include <chrono>
#include "glm/glm.hpp"

namespace util {
    long long getCurrentTimeInMs();
    std::vector<std::string> splitString(const std::string& string, char delimiter);
    void printVec3(glm::vec3 vec);
    std::string stripLastWordFromPath(const std::string& path);
    std::string normalizePath(const std::filesystem::path& path);
    std::string extractFileName(const std::string& path);
}


#endif //OPENGL_UTIL_H
