#include "game_object.hpp"

GameObject::GameObject(ResourcesManager *resourcesManager, std::string modelName) {
    this->resourcesManager = resourcesManager;
    this->position = glm::vec3(0, 0, 0);
    this->rotate = glm::vec3(0, 0, 0);
    this->scale = glm::vec3(1, 1, 1);

    this->model = resourcesManager->getModel(modelName);
}

// TODO: shaders should be provided by materials
void GameObject::draw(Camera *camera, Shader *shader) {
    // glBindVertexArray(VAO);
   

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->position);
    model = glm::scale(model, this->scale);
	// TODO: impl rotating model = glm::rotate()

    // TODO: move binding to material
	// Bind model matrix
	shader->setMat4("model", model);

    // Material bind
	shader->setVec3("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
	shader->setVec3("material.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
	shader->setVec3("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
	shader->setFloat("material.shininess", 76.8f);

    this->model->Draw(*shader);
}