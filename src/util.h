//
// Created by Sebastian on 16.02.2024.
//

#ifndef OPENGL_UTIL_H
#define OPENGL_UTIL_H
#include <chrono>


namespace util {
    long long getCurrentTimeInMs();
    std::vector<std::string> splitString(const std::string& string, char delimiter);
}


#endif //OPENGL_UTIL_H
