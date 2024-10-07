#ifndef H_BASICGAMEOBJECT
#define H_BASICGAMEOBJECT

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <model.hpp>
#include <camera.hpp>
#include "resources_manager.hpp"
#include "gameobject.hpp"

class BasicGameObject : public GameObject {
public:
    BasicGameObject(
        std::string name,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        Model *model,
        Shader *shader
    );

    // std::string getName();

    // glm::vec3 getPosition();
    // glm::vec3 getRotation();
    // glm::vec3 getScale();

    // Model *getModel();
    // Shader *getShader();
};

#endif