#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

class Grid {
public:
    Grid(int size = 20, float step = 1.0f);
    void draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
    ~Grid();
private:
    GLuint VAO, VBO;
    int lineCount;
};
