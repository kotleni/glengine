#include "renderer.hpp"

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0;
glm::mat4 directionalLightTransform;

Renderer::Renderer(SDL_Window *window, SDL_GLContext gl_context) {
    this->gl_context = gl_context;
    this->window = window;
    this->clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
    this->isEnableDepthBuffer = true;

    this->skybox = new Skybox();
	this->skybox->load("skybox1");
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

void Renderer::setDirectionalShadowShader(Shader *shader) {
    this->directionalShadowShader = shader;
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

void Renderer::renderShadowMap(DirectionalLight* light) {
    this->directionalShadowShader->use();

    glViewport(0, 0, light->getShadowMap()->getShadowWidth(), light->getShadowMap()->getShadowHeight());

	light->getShadowMap()->write();
	glClear(GL_DEPTH_BUFFER_BIT);

    directionalLightTransform = light->calculateLightTransform();
	uniformModel = directionalShadowShader->getModelLocation();
	directionalShadowShader->setDirectionalLightTransform(&directionalLightTransform);

	// TODO: renderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderFrame(
    Camera *camera,
    std::vector<Renderable> renderables,
    DirectionalLight *directionalLight,
    std::vector<PointLight*> pointLights,
    std::vector<SpotLight*> spotLights
){
    if(directionalLight == nullptr) {
        LOG_ERROR("Panic! No any directional light is initialized.");
        return;
        // TODO: Panic engine
    }

    skybox->draw(camera);

    // Render all renderables
    for(int i = 0; i < renderables.size(); i += 1) {
        Renderable renderable = renderables.at(i);
        Shader *shader = renderable.shader;

        // Use shader
        shader->use();
        camera->applyToShader(shader);

        // Directional light
        directionalLightTransform = directionalLight->calculateLightTransform();
        shader->setDirectionalLightTransform(&directionalLightTransform);
        shader->setDirectionalLight(directionalLight);

        // Shadow map
        directionalLight->getShadowMap()->read(GL_TEXTURE1);
        shader->setDirectionalShadowMap(GL_TEXTURE1);

        // Point lights
        if(!pointLights.empty())
            shader->setPointLights(pointLights[0], pointLights.size());

        // Spot lights
        if(!spotLights.empty())
            shader->setSpotLights(spotLights[0], spotLights.size());
   
        // Calc model
	    glm::mat4 model = glm::mat4(1.0f);
	    model = glm::translate(model, renderable.position);
        model = glm::scale(model, renderable.scale);
	    // TODO: impl rotating model = glm::rotate()

        // Apply model
        uniformModel = shader->getModelLocation();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

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