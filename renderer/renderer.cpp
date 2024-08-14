#include "renderer/renderer.hpp"

Renderer::Renderer(SDL_Window *window, SDL_GLContext gl_context) {
    this->gl_context = gl_context;
    this->window = window;
    this->clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
    this->isEnableDepthBuffer = true;
}

void Renderer::shutdown() {

}

SDL_GLContext Renderer::getGLContext() {
    return this->gl_context;
}

glm::vec2 Renderer::get_render_size() {
	int w, h;
	SDL_GL_GetDrawableSize(this->window, &w, &h);
	return glm::vec2(w, h);
}

void Renderer::beginFrame() {
    glViewport(0, 0, this->get_render_size().x, this->get_render_size().y);
	glClearColor(
        this->clearColor.x * this->clearColor.w,
		this->clearColor.y * this->clearColor.w, 
        this->clearColor.z * this->clearColor.w,
		this->clearColor.w
    );
    GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
    if(this->isEnableDepthBuffer) clearMask |= GL_DEPTH_BUFFER_BIT;
	glClear(clearMask);
}

void Renderer::endFrame(int fpsMax, bool enableVsync) {
    SDL_GL_SwapWindow(window);

    SDL_GL_SetSwapInterval(enableVsync);

    // FPS cap
	if(!enableVsync) {
		SDL_Delay(1000 / fpsMax);
	}
}

// TODO: i need to make universal Light abstract class for all lights
// TODO: and put here is struct? or smthing
void Renderer::createLight(glm::vec3 position) {
    DirectionalLight directional = new DirectionalLight(position);
    this->lights.push_back(directional);
}