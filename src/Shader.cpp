#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vFile(vertexPath);
    std::ifstream fFile(fragmentPath);

    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    std::string vCode = vStream.str();
    std::string fCode = fStream.str();

    const char* vSrc=vCode.c_str();
    const char* fSrc=fCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vSrc,nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fSrc,nullptr);
    glCompileShader(fragmentShader);
    
    id= glCreateProgram();
    glAttachShader(id,vertexShader);
    glAttachShader(id,fragmentShader);
    glLinkProgram(id);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
}
void Shader::use(){
        glUseProgram(id);
    }

void Shader::setMat4(const char* name, const glm::mat4& matrix) {
    GLint location = glGetUniformLocation(id, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
