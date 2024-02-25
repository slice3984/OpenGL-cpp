//
// Created by Sebastian on 16.02.2024.
//

#include "util.h"

namespace util {
    long long getCurrentTimeInMs() {
        return std::chrono::duration_cast<std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    std::vector<std::string> splitString(const std::string& string, char delimiter) {
        std::vector<std::string> result;
        std::string tmpStr;

        for (const char& c : string) {
            if (c == delimiter) {
                result.push_back(tmpStr);
                tmpStr = "";
            } else {
                tmpStr += c;
            }
        }

        if (!tmpStr.empty()) {
            result.push_back(tmpStr);
        }

        return result;
    }

    void printVec3(glm::vec3 vec) {
        std::cout << "Vector X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z << std::endl;
    }

    std::string stripLastWordFromPath(const std::string& path) {
        size_t lastSlashPos = path.find_last_of('/');

        if (lastSlashPos != std::string::npos) {
            return path.substr(0, lastSlashPos + 1);
        }

        return path;
    }

    std::string normalizePath(const std::filesystem::path& path) {
        std::string normalizedPath = path.string();
        std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');

        return normalizedPath;
    }

    std::string extractFileName(const std::string& path) {
        size_t separatorPos = path.find_last_of("/\\");

        std::string fileName;

        if (separatorPos != std::string::npos) {
            fileName = path.substr(separatorPos + 1);
        } else {
            fileName = path;
        }

        size_t extensionPos = fileName.rfind('.');
        if (extensionPos != std::string::npos) {
            fileName = fileName.substr(0, extensionPos);
        }

        return fileName;
    }

    ImageData loadImage(const std::string &path, bool verticalFlip) {
        int width, height, nChannels;

        stbi_set_flip_vertically_on_load(verticalFlip);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);

        if (data == nullptr) {
            return { false, nullptr, 0, 0, 0 };
        }

        return { true, data, width, height, nChannels };
    }
}

