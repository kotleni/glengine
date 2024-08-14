#include "renderer/renderer.hpp"

Renderer::Renderer(SDL_Window *window, SDL_GLContext gl_context) {
    this->gl_context = gl_context;
    this->window = window;
}

void Renderer::shutdown() {

}

SDL_GLContext Renderer::getGLContext() {
    return this->gl_context;
}