#ifndef H_RENDERER
#define H_RENDERER

#include <SDL2/SDL.h>

class Renderer {
private:
    SDL_Window *window;
    SDL_GLContext gl_context;
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
};

#endif