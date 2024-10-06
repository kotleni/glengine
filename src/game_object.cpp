#include "game_object.hpp"

GameObject::GameObject(ResourcesManager *resourcesManager, std::string modelName) {
    this->resourcesManager = resourcesManager;
    this->position = glm::vec3(0, 0, 0);
    this->rotate = glm::vec3(0, 0, 0);
    this->scale = glm::vec3(1, 1, 1);

    this->name = modelName;
    this->model = resourcesManager->getModel(modelName);
}

Model *GameObject::getModel() {
    return this->model;
}