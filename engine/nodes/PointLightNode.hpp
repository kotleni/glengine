#ifndef H_POINTLIGHTNODE
#define H_POINTLIGHTNODE

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <model.hpp>
#include <camera.hpp>
#include "../gameobject.hpp"
#include <PointLight.hpp>

class PointLightNode : public GameObject {
public:
    PointLightNode(
        std::string name,
        glm::vec3 position,
		glm::vec3 color,
    	float ambientIntensity,
    	float diffuceIntensity,
		float constant,
		float linear,
		float exponent
    );

    PointLight* pointLight;
    
	glm::vec3 color;
    float ambientIntensity;
    float diffuceIntensity;
	float constant;
	float linear;
	float exponent;
};

#endif