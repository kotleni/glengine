#ifndef H_SHADER
#define H_SHADER

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
public:
    GLuint program;

    Shader(GLint programId);
    void use();
    
    void setMat4(const char* key, glm::mat4 mat);
    void setVec3(const char* key, glm::vec3 vec);
    void setVec2(const char* key, glm::vec2 vec);
    void setFloat(const char* key, float value);

    static Shader *load(char* file);
};

#endif