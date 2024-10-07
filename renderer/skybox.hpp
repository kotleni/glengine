#ifndef H_SKYBOX
#define H_SKYBOX

#include <GL/glew.h>

#include <string>
#if defined(LINUX) | defined(WINDOWS)
#include <GL/gl.h>
#elif defined(OSX)
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#endif
#include <SDL2/SDL_opengl.h>
#include "shader.hpp"
#include "camera.hpp"

class Skybox {
private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;

    Shader* shader;
public:
    Skybox();
    void load(std::string name);
    void draw(Camera *camera);
};

#endif