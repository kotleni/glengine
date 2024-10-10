#ifndef H_RENDERABLE
#define H_RENDERABLE

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <model.hpp>
#include <shader.hpp>

/*
    Represents all abstract data what needed to render
*/
struct Renderable {
    Model *model;
    Shader *shader;
    glm::vec3 position;
    glm::vec3 scale;
    bool isEditorOnly;
    // TODO: Impl rotation
};

#endif