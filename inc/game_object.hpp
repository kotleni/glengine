#ifndef H_GAME_OBJECT
#define H_GAME_OBJECT

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "resources_manager.hpp"

class GameObject {
private:
    ResourcesManager *resourcesManager;
    Model* model;
    std::string name;

    GLuint VAO;
    GLuint VBO;
    //GLuint EBO;
public:
    glm::vec3 position;
    glm::vec3 rotate;
    glm::vec3 scale;

    GameObject(ResourcesManager *resourcesManager, std::string modelName);
    void draw(Camera *camera, Shader *shader);
};

#endif