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
}