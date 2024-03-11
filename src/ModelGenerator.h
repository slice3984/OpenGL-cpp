//
// Created by Sebastian on 11.03.2024.
//

#ifndef OPENGL_MODELGENERATOR_H
#define OPENGL_MODELGENERATOR_H
#include <string>
#include "ImageData.h"
#include "TextureStore.h"
#include "MaterialStore.h"
#include "ModelStore.h"

namespace ModelGenerator {
    void generateFloor(const std::string& modelName, float height, float width, ImageData tileTexture, float anisotropicFiltering,
                       bool centerFloor, TextureStore& textureStore, MaterialStore& materialStore, ModelStore& modelStore);
}

#endif //OPENGL_MODELGENERATOR_H
