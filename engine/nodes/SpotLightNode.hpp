#ifndef H_SPOTLIGHTNODE
#define H_SPOTLIGHTNODE

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <model.hpp>
#include <camera.hpp>
#include "../gameobject.hpp"
#include <SpotLight.hpp>

class SpotLightNode : public GameObject {
public:
    SpotLightNode(
        std::string name,
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

    SpotLight* spotLight;
    
	glm::vec3 direction;
	glm::vec3 color;
    int shadowWidth;
    int shadowHeight;
    float ambientIntensity;
    float diffuceIntensity;
	float constant;
	float linear;
	float exponent;
	float edge;
};

#endif