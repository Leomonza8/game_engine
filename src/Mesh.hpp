#pragma once
#include <glad/glad.h> 
class Mesh{
public:
    GLuint VAO, VBO;
    Mesh(float* vertices, int count);
    void draw();
    ~Mesh();
};