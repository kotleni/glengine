#ifndef H_DIRECTIONAL_LIGHT
#define H_DIRECTIONAL_LIGHT

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "renderer/shader.hpp"

class DirectionalLight {
private:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

public:
    DirectionalLight(glm::vec3 direction);
    void apply(Shader *shader);
};

#endif