#include "renderer.hpp"

Renderer::Renderer(SDL_Window *window, SDL_GLContext gl_context) {
    this->gl_context = gl_context;
    this->window = window;
    this->clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
    this->isEnableDepthBuffer = true;

    this->skybox = new Skybox();
	this->skybox->load("skybox1");

    this->createLight(glm::vec3(32, 32, 32));
}

void Renderer::shutdown() {

}

SDL_GLContext Renderer::getGLContext() {
    return this->gl_context;
}

SDL_Window *Renderer::getSDLWindow() {
    return this->window;
}

glm::vec2 Renderer::get_render_size() {
	int w, h;
	SDL_GL_GetDrawableSize(this->window, &w, &h);
	return glm::vec2(w, h);
}

void Renderer::setDefaultShader(Shader *shader) {
    this->defaultShader = shader;
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

void Renderer::renderFrame(Camera *camera, std::vector<Renderable> renderables, bool isRenderLight) {
    skybox->draw(camera);

    // Render all renderables
    for(int i = 0; i < renderables.size(); i += 1) {
        Renderable renderable = renderables.at(i);
        // glBindVertexArray(VAO);

        Shader *shader = renderable.shader;

        // Use shader
        shader->use();
        camera->applyToShader(shader);
        shader->setBool("features.is_use_light", isRenderLight);

        // Apply light to shader
        for(int i = 0; i < lights.size(); i += 1) {
            DirectionalLight light = lights.at(i);
            light.apply(shader);
        }
   
	    glm::mat4 model = glm::mat4(1.0f);
	    model = glm::translate(model, renderable.position);
        model = glm::scale(model, renderable.scale);
	    // TODO: impl rotating model = glm::rotate()

        // TODO: move binding to material
	    // Bind model matrix
	    shader->setMat4("model", model);

        // Material bind
	    shader->setVec3("material.ambient", glm::vec3(0.6f, 0.6f, 0.6f));
	    shader->setVec3("material.diffuse", glm::vec3(0.6f, 0.6f, 0.3f));
	    shader->setVec3("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
	    shader->setFloat("material.shininess", 76.8f);

        renderable.model->Draw(*shader);
    }
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
    DirectionalLight directional = DirectionalLight(position);
    this->lights.push_back(directional);

    LOG_INFO("New light source is created at ({}, {}, {}}).", position.x, position.y, position.z);
}