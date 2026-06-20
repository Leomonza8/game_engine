#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Model.hpp"
#include "Shader.hpp"

struct SceneObject {
    std::string name;
    std::string path;
    std::unique_ptr<Model> model;
};

class Scene {
public:
    std::vector<SceneObject> objects;
    int selected = -1;

    void add(const std::string& name, const std::string& path);
    void remove(int index);
    void draw(Shader& shader);
    void save(const std::string& filepath);
    void load(const std::string& filepath);
};
