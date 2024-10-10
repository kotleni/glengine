#ifndef H_DIRECTIONALLIGHT
#define H_DIRECTIONALLIGHT

#include "Light.hpp"
#include <iostream>

class DirectionalLight : public Light {
public:
	DirectionalLight();
	DirectionalLight(
		glm::vec3 direction,
    	glm::vec3 color,
    	int shadowWidth,
    	int shadowHeight,
    	float ambientIntensity,
    	float diffuceIntensity
	);

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	glm::mat4 calculateLightTransform();

	~DirectionalLight();

private:
	glm::vec3 direction;
};

#endif