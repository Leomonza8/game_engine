#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// check de erros feito pelo claude
void checkShaderError(GLuint shader, const char* tipo) {
    int sucesso;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &sucesso);
    if (!sucesso) {
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Erro no " << tipo << ": " << log << "\n";
    }
}

void checkProgramError(GLuint program) {
    int sucesso;
    char log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &sucesso);
    if (!sucesso) {
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Erro no link do programa: " << log << "\n";
    }
}

// triangulo teste ( pontos)
float vertices[]{
    -0.5f,-0.5f,0.0,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0); // laranja
    }
)";

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

    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderError(vertexShader, "vertex shader");

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource,nullptr);
    glCompileShader(fragmentShader);
    checkShaderError(fragmentShader, "fragment shader");

    // Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramError(shaderProgram);

    // shaders individuais não são mais necessários após o link
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO;
    glGenVertexArrays(1,&VAO);
    GLuint VBO;
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    
    //loop para a janela
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window); //exibe o que foi desenhado
        glfwPollEvents(); //eventos 
    }
    // fecha o window
    glfwTerminate();
    return 0;
}
