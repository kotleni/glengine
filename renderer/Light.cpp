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
	colour = color;
	ambientIntensity = ambientIntensity;
	diffuseIntensity = diffuceIntensity;

	shadowMap = new ShadowMap();
	shadowMap->init(shadowWidth, shadowHeight);
}

Light::~Light() {
}