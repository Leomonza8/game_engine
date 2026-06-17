#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    GLuint id;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setMat4(const char* name, const glm::mat4& matrix);
};
