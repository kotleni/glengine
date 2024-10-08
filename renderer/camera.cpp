#include "camera.hpp"

const float cameraSpeed = 0.05f;
const float cameraSensitivity = 0.04f;
const float fov = 70.0f;
const float minPitch = -89.0f;
const float maxPitch = 89.0f;

Camera::Camera(glm::vec2 render_size) {
    proj = glm::perspective(glm::radians(fov), (float)render_size.x / (float)render_size.y, 0.1f, 1000.0f);
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
    // Limit pitch
    if(this->pitch < minPitch)
        this->pitch = minPitch;
    else if(this->pitch > maxPitch)
        this->pitch = maxPitch;

    glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void Camera::applyToShader(Shader *shader) {
    auto uniformProjection = shader->getProjectionLocation();
    auto uniformView = shader->getViewLocation();
    auto uniformEyePosition = shader->getEyePositionLocation();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(this->get_projection()));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(this->get_view()));
	glUniform3f(uniformEyePosition, this->get_position().x, this->get_position().y, this->get_position().z);
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