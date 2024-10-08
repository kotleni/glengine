#ifndef H_DIRECTIONALLIGHTNODE
#define H_DIRECTIONALLIGHTNODE

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <model.hpp>
#include <camera.hpp>
#include "../gameobject.hpp"

class DirectionalLightNode : public GameObject {
public:
    DirectionalLightNode(
        std::string name,
        glm::vec3 direction,
        glm::vec3 color,
        int shadowWidth,
        int shadowHeight,
        float ambientIntensity,
        float diffuceIntensity
    );

    DirectionalLight* directionalLight;
    glm::vec3 direction;
    glm::vec3 color;
    glm::vec2 shadowSize;
    glm::vec2 intensity;
};

#endif