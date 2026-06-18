#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Camera::Camera(glm::vec3 pos){
    position =pos;
    front = glm::vec3 (0.0f,0.0f,-1.0f);
    up =glm::vec3(0.0f,1.0f,0.0f);
    yaw =-90.0;
    pitch =0.0f;
}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(position,position+front,up);
}
void Camera::processKeyboard(GLFWwindow* window, float speed){
    front = glm::normalize(front);
    glm::vec3 right = glm::normalize(glm::cross(front,up));
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
        position += front*speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        position -= up * speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += up * speed;
}

void Camera::processMouse(float xoffset, float yoffset) {
    float sensitivity = 0.1f;
    yaw   += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
}
