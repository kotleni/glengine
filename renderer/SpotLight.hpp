#ifndef H_SPOTLIGHT
#define H_SPOTLIGHT

#include "PointLight.hpp"

class SpotLight : public PointLight {
public:
	SpotLight();

	SpotLight(
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
	);

	void useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	void setFlash(glm::vec3 pos, glm::vec3 dir);

	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};

#endif