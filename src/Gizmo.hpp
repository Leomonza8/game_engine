#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

enum class GizmoAxis { X, Y, Z };

class Gizmo {
public:
    Gizmo();
    void draw(Shader& shader, const glm::vec3& position,
              const glm::mat4& view, const glm::mat4& projection);
    ~Gizmo();
private:
    GLuint VAO, VBO;
    void drawArrow(Shader& shader, const glm::mat4& base,
                   GizmoAxis axis, const glm::vec3& color,
                   const glm::mat4& view, const glm::mat4& projection);
};
