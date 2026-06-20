#include "Grid.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

Grid::Grid(int size, float step) {
    std::vector<float> verts;
    float half = size * step;

    for (int i = -size; i <= size; i++) {
        float p = i * step;
        // linha paralela ao Z
        verts.insert(verts.end(), { -half, 0.0f, p,  half, 0.0f, p });
        // linha paralela ao X
        verts.insert(verts.end(), { p, 0.0f, -half,  p, 0.0f, half });
    }

    lineCount = verts.size() / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Grid::draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec4("gridColor", glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, lineCount);
}

Grid::~Grid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
