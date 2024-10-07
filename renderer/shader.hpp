#ifndef H_SHADER
#define H_SHADER

#include <GL/glew.h>

#if defined(LINUX) | defined(WINDOWS)
#include <GL/gl.h>
#elif defined(OSX)
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#endif
#include <SDL2/SDL_opengl.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <logger.hpp>

class Shader {
public:
    GLuint program;

    Shader(GLint programId);
    void use();
    
    void setMat4(const char* key, glm::mat4 mat);
    void setVec3(const char* key, glm::vec3 vec);
    void setVec2(const char* key, glm::vec2 vec);
    void setFloat(const char* key, float value);
    void setBool(const char* key, bool value);
    void setInt(const char* key, int value);

    static Shader *load(std::string file);
};

#endif