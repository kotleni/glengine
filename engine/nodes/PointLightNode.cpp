#include "PointLightNode.hpp"

PointLightNode::PointLightNode(
    std::string name,
    glm::vec3 position,
	glm::vec3 color,
    float ambientIntensity,
    float diffuceIntensity,
	float constant,
	float linear,
	float exponent
) : GameObject(
    name,
    position,
    glm::vec3(),
    glm::vec3(),
    nullptr,
    nullptr
) { 
    this->pointLight = new PointLight{
        position, 
        color,
        ambientIntensity, 
        diffuceIntensity,
        constant,
        linear,
        exponent
    };
    
    this->color = color;
    this->ambientIntensity = ambientIntensity;
    this->diffuceIntensity = diffuceIntensity;
    this->constant = constant;
    this->linear = linear;
    this->exponent = exponent;
}