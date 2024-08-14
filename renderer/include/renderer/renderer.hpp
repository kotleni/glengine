#ifndef H_RENDERER
#define H_RENDERER

#include <vector>

#include <SDL2/SDL.h>
#include <GL/gl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <renderer/directional_light.hpp>

class Renderer {
private:
    SDL_Window *window;
    SDL_GLContext gl_context;

    glm::vec4 clearColor;
    bool isEnableDepthBuffer;

    std::vector<DirectionalLight> lights;
public:
    Renderer(SDL_Window *window, SDL_GLContext gl_context);
    void shutdown();

    /***
     * Get actual sdl opengl context.
     * 
     * @deprecated Renderer should'nt present any internal things
     * @return Actual SDL_GLContext
     * ***/
    SDL_GLContext getGLContext();

    glm::vec2 get_render_size();

    void beginFrame();
    void endFrame(int fpsMax, bool enableVsync);

    void createLight(glm::vec3 position);
};

#endif