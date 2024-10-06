#include "directional_light.hpp"

DirectionalLight::DirectionalLight(glm::vec3 direction) {
    this->direction = direction;
    this->ambient = glm::vec3(0.7f, 0.7f, 0.7f);
    this->diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
    this->specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void DirectionalLight::apply(Shader *shader) {
    shader->setVec3("light.direction", this->direction);
	shader->setVec3("light.ambient", this->ambient);
	shader->setVec3("light.diffuse", this->diffuse);
	shader->setVec3("light.specular", this->specular);
}