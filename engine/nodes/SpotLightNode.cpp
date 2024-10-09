#include "SpotLightNode.hpp"

SpotLightNode::SpotLightNode(
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
) : GameObject(
    name,
    position,
    glm::vec3(),
    glm::vec3(),
    nullptr,
    nullptr
) { 
    this->spotLight = new SpotLight{
        position, 
        direction,
        color,
        shadowWidth,
        shadowHeight,
        ambientIntensity,
        diffuceIntensity,
        constant,
        linear,
        exponent,
        edge
    };
    
    this->direction = direction;
    this->color = color;
    this->shadowWidth = shadowWidth;
    this->shadowHeight = shadowHeight;
    this->ambientIntensity = ambientIntensity;
    this->diffuceIntensity = diffuceIntensity;
    this->constant = constant;
    this->linear = linear;
    this->exponent = exponent;
    this->edge = edge;
}