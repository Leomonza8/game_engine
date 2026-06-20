#pragma once
#include <glad/glad.h>
#include <vector>

class Mesh {
public:
    GLuint VAO, VBO;
    int vertexCount;
    // stride: 8 floats — X Y Z  U V  NX NY NZ
    Mesh(const std::vector<float>& vertices);
    void draw();
    ~Mesh();
};
