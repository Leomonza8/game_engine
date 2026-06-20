#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "Mesh.hpp"
#include "Shader.hpp"

struct MeshEntry {
    std::unique_ptr<Mesh> mesh;
    GLuint texture;
};

class Model {
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale    = glm::vec3(1.0f);

    Model(const std::string& path);
    void draw(Shader& shader);

private:
    std::vector<MeshEntry> entries;
    std::string directory;

    GLuint loadTexture(const std::string& path);
    GLuint loadTextureEmbedded(const aiTexture* tex);
};
