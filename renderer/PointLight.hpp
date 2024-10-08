#ifndef H_POINTLIGHT
#define H_POINTLIGHT

#include "Light.hpp"

class PointLight : public Light {
public:
	PointLight();
	PointLight(
		glm::vec3 position,
		glm::vec3 color,
    	float ambientIntensity,
    	float diffuceIntensity,
		float constant,
		float linear,
		float exponent
	);

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat positionLocation,
		GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};

#endif