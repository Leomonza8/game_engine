#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Shader.hpp"
#include "Scene.hpp"
#include "Grid.hpp"
#include "Gizmo.hpp"
#include "camera.hpp"

static const std::string ROOT = ASSETS_PATH;

Camera camera(glm::vec3(0.0f, 2.0f, 5.0f));
float lastX = 960.0f, lastY = 540.0f;
bool firstMouse = true;
bool cursorLocked = true;
float cameraSpeed = 5.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!cursorLocked) return;
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    camera.processMouse(xpos - lastX, lastY - ypos);
    lastX = xpos; lastY = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        cursorLocked = !cursorLocked;
        firstMouse = true;
        glfwSetInputMode(window, GLFW_CURSOR,
            cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Game Engine", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Shader shader(
        (ROOT + "shaders/vertex.glsl").c_str(),
        (ROOT + "shaders/fragment.glsl").c_str()
    );
    Shader gridShader(
        (ROOT + "shaders/grid_vertex.glsl").c_str(),
        (ROOT + "shaders/grid_fragment.glsl").c_str()
    );
    Shader gizmoShader(
        (ROOT + "shaders/gizmo_vertex.glsl").c_str(),
        (ROOT + "shaders/gizmo_fragment.glsl").c_str()
    );

    shader.use();
    shader.setInt("tex", 0);

    Grid  grid(30, 1.0f);
    Gizmo gizmo;
    Scene scene;

    static char pathBuf[256]  = "";
    static char nameBuf[64]   = "";
    static char scenePath[256] = "scene.json";

    glm::vec3 lightPos(5.0f, 8.0f, 5.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    bool showGrid = true;

    float lastTime = 0.0f, deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPollEvents();
        if (cursorLocked)
            camera.processKeyboard(window, cameraSpeed * deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(360, 600), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::Begin("Engine");

        ImGui::Text("ESC = alternar cursor/camera");
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Importar Modelo", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::InputText("Nome##imp", nameBuf, sizeof(nameBuf));
            ImGui::InputText("Caminho##imp", pathBuf, sizeof(pathBuf));
            if (ImGui::Button("Importar")) {
                std::string name = nameBuf;
                std::string fullPath = ROOT + pathBuf;
                if (name.empty()) {
                    std::string p = pathBuf;
                    auto slash = p.find_last_of('/');
                    auto dot   = p.find_last_of('.');
                    size_t start = (slash == std::string::npos) ? 0 : slash + 1;
                    size_t len   = (dot == std::string::npos || dot <= start)
                                   ? std::string::npos : dot - start;
                    name = p.substr(start, len);
                    if (name.empty()) name = p;
                }
                scene.add(name, fullPath);
                nameBuf[0] = '\0';
            }
        }

        if (ImGui::CollapsingHeader("Cena", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < (int)scene.objects.size(); i++) {
                bool sel = (scene.selected == i);
                if (ImGui::Selectable(scene.objects[i].name.c_str(), sel))
                    scene.selected = i;
            }
        }

        if (scene.selected >= 0 && scene.selected < (int)scene.objects.size()) {
            auto& obj = *scene.objects[scene.selected].model;
            ImGui::Separator();
            ImGui::Text("Transform: %s", scene.objects[scene.selected].name.c_str());
            ImGui::DragFloat3("Posicao", &obj.position.x, 0.05f);
            ImGui::DragFloat3("Rotacao", &obj.rotation.x, 1.0f);
            ImGui::DragFloat3("Escala",  &obj.scale.x,    0.01f, 0.001f, 100.0f);
            if (ImGui::Button("Remover")) scene.remove(scene.selected);
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Ambiente")) {
            ImGui::ColorEdit3("Luz cor",     &lightColor.x);
            ImGui::DragFloat3("Luz posicao", &lightPos.x, 0.1f);
            ImGui::Checkbox("Grid", &showGrid);
            ImGui::SliderFloat("Velocidade cam", &cameraSpeed, 0.5f, 50.0f);
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Cena arquivo")) {
            ImGui::InputText("Arquivo", scenePath, sizeof(scenePath));
            if (ImGui::Button("Salvar"))   scene.save(ROOT + scenePath);
            ImGui::SameLine();
            if (ImGui::Button("Carregar")) scene.load(ROOT + scenePath);
        }

        ImGui::End();

        // --- Render ---
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int fbW, fbH;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
        float aspect = (fbH > 0) ? (float)fbW / (float)fbH : 1.0f;

        glm::mat4 view       = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 200.0f);

        if (showGrid)
            grid.draw(gridShader, view, projection);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("lightPos",   lightPos);
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("viewPos",    camera.position);
        scene.draw(shader);

        // gizmo no objeto selecionado
        if (scene.selected >= 0 && scene.selected < (int)scene.objects.size()) {
            auto& pos = scene.objects[scene.selected].model->position;
            gizmo.draw(gizmoShader, pos, view, projection);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
