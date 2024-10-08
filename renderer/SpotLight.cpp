#include "SpotLight.hpp"

SpotLight::SpotLight() : PointLight() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(
	glm::vec3 position,
	glm::vec3 direction,
	glm::vec3 color,
    int shadowWidth,
    int shadowHeight,
    float ambientIntensity,
    float diffuceIntensity,
	float constant,
	float linear,
	float exponent,
	float edge
) : PointLight(position, color, ambientIntensity, diffuceIntensity, constant, linear, exponent) {
	direction = glm::normalize(direction);

	this->edge = edge;
	procEdge = cosf(glm::radians(this->edge));
}

void SpotLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
	GLuint edgeLocation) {
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir) {
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight() {
}