#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "Mesh.hpp"

// triangulo teste ( pontos)
float vertices[]{
    -0.5f,-0.5f,0.0,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

int main() {
    //inicialização do opengl
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

    GLFWwindow* window = glfwCreateWindow(800,600,"Game Engine",nullptr,nullptr);
    if(!window){
        std::cerr << "Falha ao criar janela\n";
        glfwTerminate();
        return -1; // para o codigo
    }

    // janela de contexto
    glfwMakeContextCurrent(window);

    // carregar os drivers do opengl
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD\n";
        return -1;
    }
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Mesh mesh(vertices,3);
    
    //loop para a janela
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // glUseProgram(shaderProgram);
        shader.use();
        mesh.draw();
        glfwSwapBuffers(window); //exibe o que foi desenhado
        glfwPollEvents(); //eventos 
    }
    // fecha o window
    glfwTerminate();
    return 0;
}
