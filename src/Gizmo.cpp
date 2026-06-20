#include "Gizmo.hpp"
#include <glm/gtc/matrix_transform.hpp>

static float arrowVerts[] = {
    // eixo
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    // ponta (cone simplificado — 4 linhas)
    0.0f,  1.0f,  0.0f,   -0.06f, 0.85f,  0.06f,
    0.0f,  1.0f,  0.0f,    0.06f, 0.85f,  0.06f,
    0.0f,  1.0f,  0.0f,   -0.06f, 0.85f, -0.06f,
    0.0f,  1.0f,  0.0f,    0.06f, 0.85f, -0.06f,
};

Gizmo::Gizmo() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowVerts), arrowVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Gizmo::drawArrow(Shader& shader, const glm::mat4& base,
                      GizmoAxis axis, const glm::vec3& color,
                      const glm::mat4& view, const glm::mat4& projection) {
    glm::mat4 rot = glm::mat4(1.0f);
    if (axis == GizmoAxis::X)
        rot = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 0, 1));
    else if (axis == GizmoAxis::Z)
        rot = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));

    shader.setMat4("model", base * rot);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("color", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 10);
}

void Gizmo::draw(Shader& shader, const glm::vec3& position,
                 const glm::mat4& view, const glm::mat4& projection) {
    glDisable(GL_DEPTH_TEST);
    shader.use();

    // escala fixa pra gizmo não encolher com a distância
    glm::mat4 base = glm::translate(glm::mat4(1.0f), position);
    base = glm::scale(base, glm::vec3(0.5f));

    drawArrow(shader, base, GizmoAxis::X, glm::vec3(1, 0, 0), view, projection);
    drawArrow(shader, base, GizmoAxis::Y, glm::vec3(0, 1, 0), view, projection);
    drawArrow(shader, base, GizmoAxis::Z, glm::vec3(0, 0, 1), view, projection);

    glEnable(GL_DEPTH_TEST);
}

Gizmo::~Gizmo() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
