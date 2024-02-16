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
}