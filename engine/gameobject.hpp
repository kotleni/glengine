#ifndef H_GAME_OBJECT
#define H_GAME_OBJECT

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <model.hpp>

class GameObject {
private:
    std::string name;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Model *model;
    Shader *shader;
public:
    GameObject(
        std::string name,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        Model *model,
        Shader *shader
    ) {
        this->name = name;
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
        this->model = model;
        this->shader = shader;
    }

    virtual std::string getName() { return this->name; };

    virtual glm::vec3 getPosition() { return this->position; }
    virtual glm::vec3 getRotation() { return this->rotation; }
    virtual glm::vec3 getScale() { return this->scale; }

    virtual Model *getModel() { return this->model; }
    virtual Shader *getShader() { return this->shader; }
};

#endif