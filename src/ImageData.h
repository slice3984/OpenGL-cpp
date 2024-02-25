//
// Created by Sebastian on 24.02.2024.
//

#ifndef OPENGL_IMAGEDATA_H
#define OPENGL_IMAGEDATA_H


#include "stb_image.h"
#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include <utility>
#include <string>

struct ImageData {
    bool loadedImage;
    unsigned char* imageData;
    int width;
    int height;
    int nChannels;
};


#endif //OPENGL_IMAGEDATA_H
