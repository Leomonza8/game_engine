#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>
#include "Shader.hpp"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    void draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
    ~Skybox();
private:
    GLuint VAO, VBO, cubemapTex;
};
