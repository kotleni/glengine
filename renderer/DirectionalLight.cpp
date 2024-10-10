#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(
	glm::vec3 direction,
    glm::vec3 color,
    int shadowWidth,
    int shadowHeight,
    float ambientIntensity,
    float diffuceIntensity
) : Light(color, shadowWidth, shadowHeight, ambientIntensity, diffuceIntensity) {
	this->direction = direction;

	this->lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::calculateLightTransform() {
	glm::vec3 eye = -direction;
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Check for zero-length vectors
    if (glm::length(eye - center) == 0.0f || glm::length(up) == 0.0f) {
        std::cout << "Invalid vectors for lookAt function, might cause NaN." << std::endl;
    }

    return lightProj * glm::lookAt(eye, center, up);
}

DirectionalLight::~DirectionalLight() {
}