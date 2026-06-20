#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;

    explicit Camera(glm::vec3 position);
    glm::mat4 getViewMatrix() const;
    void processKeyboard(GLFWwindow* window, float speed);
    void processMouse(float xoffset, float yoffset);
};