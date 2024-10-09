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

#include <ShadowMap.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <SpotLight.hpp>

#include <skybox.hpp>

#include <renderable.hpp>
#include <logger.hpp>

class Renderer {
private:
    SDL_Window *window;
    SDL_GLContext gl_context;

    glm::vec4 clearColor;
    bool isEnableDepthBuffer;

    Shader *directionalShadowShader;
    Skybox *skybox;

    void renderPass(
        Camera *camera,
        std::vector<Renderable> renderables,
        DirectionalLight *directionalLight,
        std::vector<PointLight*> pointLights,
        std::vector<SpotLight*> spotLights,
        bool isForBackingShadowMap
    );

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

    void setDirectionalShadowShader(Shader *shader);

    void beginFrame();

    void renderShadowMap(DirectionalLight* light, std::vector<Renderable> renderables);

    /* 
        Render single frame for camera

        @param camera Camera to rendering into using parameters.
        @param renderables List of prepared renderables.
        @param directionalLight Single point of truth about dir light.
        @param pointsLigts List of point lights.
        @param spotLights List of spot lights.

        @attention Thanks Skrillex, for awesome Dubstep.
    */
    void renderFrame(
        Camera *camera,
        std::vector<Renderable> renderables,
        DirectionalLight *directionalLight,
        std::vector<PointLight*> pointLights,
        std::vector<SpotLight*> spotLights
    );

    void endFrame(int fpsMax, bool enableVsync);
};

#endif