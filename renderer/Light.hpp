#ifndef H_LIGHT
#define H_LIGHT

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.hpp"

class Light {
public:
	Light();
	Light(
    	glm::vec3 color,
    	int shadowWidth,
    	int shadowHeight,
    	float ambientIntensity,
    	float diffuceIntensity
	);

	ShadowMap* getShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};

#endif