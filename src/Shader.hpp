#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    GLuint id = 0;
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use();
    void setMat4(const char* name, const glm::mat4& matrix);
    void setInt(const char* name, int value);
    void setBool(const char* name, bool value);
    void setVec3(const char* name, const glm::vec3& value);
    void setVec4(const char* name, const glm::vec4& value);
    void setMat3(const char* name, const glm::mat3& matrix);
};
