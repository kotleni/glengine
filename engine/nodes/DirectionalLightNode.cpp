#include "DirectionalLightNode.hpp"

DirectionalLightNode::DirectionalLightNode(
    std::string name,
    glm::vec3 direction,
    glm::vec3 color,
    int shadowWidth,
    int shadowHeight,
    float ambientIntensity,
    float diffuceIntensity
) : GameObject(
    name,
    glm::vec3(),
    glm::vec3(),
    glm::vec3(),
    nullptr,
    nullptr
) { 
    this->directionalLight = new DirectionalLight{
        direction, 
        color,
        shadowWidth, 
        shadowHeight,
        ambientIntensity,
        diffuceIntensity
    };
    this->direction = direction;
    this->color = color;
    this->shadowSize = shadowSize;
    this->intensity = intensity;
}