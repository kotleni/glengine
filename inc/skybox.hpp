#ifndef H_SKYBOX
#define H_SKYBOX

#include <string>
#include <GL/glew.h>
#include <SDL_opengl.h>
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