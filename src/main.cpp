#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
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
    //loop para a janela
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // fecha o window
    glfwTerminate();
    return 0;
}
