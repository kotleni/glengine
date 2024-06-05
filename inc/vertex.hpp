#ifndef H_VERTEX
#define H_VERTEX

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

#endif