#include "basicgameobject.hpp"

BasicGameObject::BasicGameObject(
    std::string name,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale,
    Model *model,
    Shader *shader
) : GameObject(
    name,
    position,
    rotation,
    scale,
    model,
    shader
) { }