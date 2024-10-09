#include "Light.hpp"

Light::Light() {
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(
    glm::vec3 color,
    int shadowWidth,
    int shadowHeight,
    float ambientIntensity,
    float diffuceIntensity
) {
	this->colour = color;
	this->ambientIntensity = ambientIntensity;
	this->diffuseIntensity = diffuceIntensity;

	this->shadowMap = new ShadowMap();
	this->shadowMap->init(shadowWidth, shadowHeight);
}

Light::~Light() {
}