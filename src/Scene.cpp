#include "Scene.hpp"
#include "../external/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void Scene::add(const std::string& name, const std::string& path) {
    objects.push_back({ name, path, std::make_unique<Model>(path) });
}

void Scene::remove(int index) {
    if (index >= 0 && index < (int)objects.size()) {
        objects.erase(objects.begin() + index);
        if (selected >= (int)objects.size())
            selected = (int)objects.size() - 1;
    }
}

void Scene::draw(Shader& shader) {
    for (auto& obj : objects)
        obj.model->draw(shader);
}

void Scene::save(const std::string& filepath) {
    json j;
    for (auto& obj : objects) {
        auto& m = *obj.model;
        j["objects"].push_back({
            {"name", obj.name},
            {"path", obj.path},
            {"position", {m.position.x, m.position.y, m.position.z}},
            {"rotation", {m.rotation.x, m.rotation.y, m.rotation.z}},
            {"scale",    {m.scale.x,    m.scale.y,    m.scale.z}}
        });
    }
    std::ofstream f(filepath);
    f << j.dump(2);
    std::cout << "Cena salva em " << filepath << "\n";
}

void Scene::load(const std::string& filepath) {
    std::ifstream f(filepath);
    if (!f) { std::cerr << "Nao foi possivel abrir " << filepath << "\n"; return; }
    try {
        json j = json::parse(f);
        objects.clear();
        selected = -1;
        for (auto& o : j["objects"]) {
            add(o["name"], o["path"]);
            auto& m = *objects.back().model;
            m.position = { o["position"][0], o["position"][1], o["position"][2] };
            m.rotation = { o["rotation"][0], o["rotation"][1], o["rotation"][2] };
            m.scale    = { o["scale"][0],    o["scale"][1],    o["scale"][2]    };
        }
        std::cout << "Cena carregada de " << filepath << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Erro ao carregar cena: " << e.what() << "\n";
    }
}
