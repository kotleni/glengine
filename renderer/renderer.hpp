#ifndef H_RENDERER
#define H_RENDERER

#include <vector>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#if defined(LINUX) | defined(WINDOWS)
#include <GL/gl.h>
#elif defined(OSX)
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <directional_light.hpp>
#include <skybox.hpp>

#include <renderable.hpp>
#include <logger.hpp>

class Renderer {
private:
    SDL_Window *window;
    SDL_GLContext gl_context;

    glm::vec4 clearColor;
    bool isEnableDepthBuffer;

    Shader *defaultShader;
    Skybox *skybox;
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
    SDL_Window *getSDLWindow();

    glm::vec2 get_render_size();

    void setDefaultShader(Shader *shader);

    void beginFrame();
    void renderFrame(Camera *camera, std::vector<Renderable> renderables, bool isRenderLight);
    void endFrame(int fpsMax, bool enableVsync);

    void createLight(glm::vec3 position);
};

#endif