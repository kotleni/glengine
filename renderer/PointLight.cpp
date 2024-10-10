#include "PointLight.hpp"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(
	glm::vec3 position,
	glm::vec3 color,
    float ambientIntensity,
    float diffuceIntensity,
	float constant,
	float linear,
	float exponent
) : Light(color, 1024, 1024, ambientIntensity, diffuceIntensity) {
	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;
}

void PointLight::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, 
							GLfloat diffuseIntensityLocation, GLfloat positionLocation, 
							GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation) {
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight() {
}