#include "camera.hpp"

// TODO: we need a single point of true (also in engine.cpp)
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

const float cameraSpeed = 0.05f;
const float cameraSensitivity = 0.04f;
const float fov = 70.0f;

Camera::Camera() {
    proj = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
}

void Camera::look_relative(float xrel, float yrel) {
    yaw += xrel * cameraSensitivity;
    pitch -= yrel * cameraSensitivity;
}

void Camera::move_forward(float step) {
    cameraPos += (cameraSpeed * cameraFront) * step;
}

void Camera::move_right(float step) {
    cameraPos += (glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed) * step;
}

void Camera::update() {
    glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

glm::mat4 Camera::get_projection() {
    return proj;
}

glm::mat4 Camera::get_view() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::vec3 Camera::get_position() {
    return cameraPos;
}