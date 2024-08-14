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
    SDL_GLContext getGLContext();
};

#endif