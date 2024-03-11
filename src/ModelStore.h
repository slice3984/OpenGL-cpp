//
// Created by Sebastian on 04.03.2024.
//

#ifndef OPENGL_MODELSTORE_H
#define OPENGL_MODELSTORE_H
#include <unordered_map>
#include <string>
#include "Model.h"

class ModelStore {
private:
    std::unordered_map<std::string, Model> models;

public:
    Model& addModel(const std::string& name, Model model);
    Model& getModel(const std::string& name);
};


#endif //OPENGL_MODELSTORE_H
