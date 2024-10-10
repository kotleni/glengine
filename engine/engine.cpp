#include "engine.hpp"

// FIXME: removed here, bcs has is anywhere
// FIXME: write an image loader 
// NOTE: this header is header only lib
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

// NOTE: this header is header only lib
#include "args.hpp"

int moveFront = 0;
int moveRight = 0;

Camera *camera;

Engine::Engine() {
	instance = this;
}

#if !defined(OSX)
void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  //fprintf( stderr, "GL: {}\n", message );
  LOG_ERROR("GL: {}\n", message);
}
#endif

bool processKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority) {
	// TODO: Check is context is not null from if (!engine().)
	// 	return true;
}

void Engine::init(int argc, char ** argv) {
    LOG_INFO("Initializing engine...");

	const std::vector<std::string> str_args(argv + 1, argv + argc);

	args::ArgumentParser parser("Engine", "Description here");
    args::Flag vsync(parser, "vsync", "Enable V-Sync in render loop.", {"vsync"});

	std::vector<std::string>::const_iterator args = parser.ParseArgs(str_args);
	
	// Init props default values
	props.is_vsync = bool{vsync};
	props.max_fps = 200;
	#ifdef ENABLE_TOOLS
		props.is_tools_mode = true;
	#else
		props.is_tools_mode = false;
	#endif

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
		0) {
		LOG_ERROR("SDL Error: {}\n", SDL_GetError());
		return;
	}
	// GL 3.0 + GLSL 130
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	// #ifdef OS_MACOS
	// 	SDL_GL_SetAttribute(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
	// #endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE //| SDL_WINDOW_ALLOW_HIGHDPI
	);
	glm::vec2 window_size = glm::vec2(1200, 800);
	window = SDL_CreateWindow(ENGINE_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size.x, window_size.y,
		window_flags);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, gl_context);

	// Glew
	glewExperimental = GL_TRUE; 
	glewInit();

	LOG_INFO("Glew initialized.");

	glEnable(GL_DEPTH_TEST);
	#if defined(ENABLE_GL_DEBUGGING) && !defined(OS_MACOS)
		LOG_INFO("OpenGL debugging is enabled in build.");
		// INFO: macOS don't support debug opengl
		glEnable              ( GL_DEBUG_OUTPUT );
		glDebugMessageCallback( MessageCallback, 0 );
	#endif

	this->renderer = new Renderer(window, gl_context);
	LOG_INFO("Renderer initialized.");
}

void Engine::init_gui() {
	glm::vec2 render_size = engine()->renderer->get_render_size();

	if (!Backend::Initialize(render_size.x, render_size.y, renderer->getSDLWindow(), renderer->getGLContext()))
	{
		// TODO: Crash
		LOG_ERROR("Backend::Initialize return false.");
		return;
	}

	// Install the custom interfaces constructed by the backend before initializing RmlUi.
	Rml::SetSystemInterface(Backend::GetSystemInterface());
	Rml::SetRenderInterface(Backend::GetRenderInterface());

	// RmlUi initialisation.
	Rml::Initialise();

	rmlContext = Rml::CreateContext("default", Rml::Vector2i(render_size.x, render_size.y));

	// TODO: HiDPI support for UI
	// rmlContext->SetDensityIndependentPixelRatio(2.0f);

	// Load all fonts
	const Rml::String directory = "../assets/ui/fonts/";

	struct FontFace {
		const char* filename;
		bool fallback_face;
	};
	FontFace font_faces[] = {
		{"LatoLatin-Regular.ttf", false},
		{"LatoLatin-Italic.ttf", false},
		{"LatoLatin-Bold.ttf", false},
		{"LatoLatin-BoldItalic.ttf", false},
	};

	LOG_INFO("Loading fonts from {}..", directory);

	for (const FontFace& face : font_faces)
		Rml::LoadFontFace(directory + face.filename, face.fallback_face);

	// Loading gui elements
	this->guiElements = new std::vector<BaseGuiElement*>();
	this->guiElements->push_back(new ConsoleGuiElement());

	for(int i = 0; i < this->guiElements->size(); i += 1) {
		this->guiElements->at(i)->load(this->rmlContext);
	}

	LOG_INFO("Loaded {} gui elements.", this->guiElements->size());
}

void Engine::render_splash() {
	// TODO: Impl rendering loading splash
}

GameObject *Engine::createGameObject(
    std::string name,
    std::string modelName,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale
) {
	Shader *shader = resourcesMamanger->getShader("default");
	Model *model = resourcesMamanger->getModel(modelName);
	GameObject *gObject = new BasicGameObject(
		name,
		position,
		rotation,
		scale,
		model,
		shader
	);
	gameObjects->push_back(gObject);
	return gObject;
}

void Engine::run() {
	render_splash();

	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Loading game objects
	this->gameObjects = new std::vector<GameObject*>();

	resourcesMamanger = new ResourcesManager();
	resourcesMamanger->loadAll();

	Shader *directionalShadowShader = resourcesMamanger->getShader("directional_shadow_map");
	renderer->setDirectionalShadowShader(directionalShadowShader);
	
	// TODO: Load from level file
	this->createGameObject(
		"Sponza",
		"../assets/models/Sponza/sponza.obj",
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0.09, 0.09, 0.09)
	);

	DirectionalLightNode *directionalLightNode = new DirectionalLightNode(
		"DirectionalLight",
		glm::vec3(0.0f, -10.0f, 5.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		2048, 2048,
		0.8f, 0.6f
	);
	this->gameObjects->push_back(directionalLightNode);

	PointLightNode *pointLightNode = new PointLightNode(
		"PointLight",
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.2f, 0.2f),
		0.6f, 0.5f,
		0.3f, 0.2f, 0.1f
	);
	this->gameObjects->push_back(pointLightNode);

	SpotLightNode *spotLightNode = new SpotLightNode(
		"SpotLight",
		glm::vec3(0.0f, 15.0f, 0.0f), // pos
		glm::vec3(0.0f, 0.0f, 0.0f), // dir
		glm::vec3(0.2f, 1.0f, 0.2f), // color
		2048, 2048,
		0.6f, 0.5f,
		0.3f, 0.2f, 0.1f,
		20.0f
	);
	this->gameObjects->push_back(spotLightNode);

	glm::vec2 render_size = engine()->renderer->get_render_size();
	camera = new Camera(render_size);

	Model *cubeModel = resourcesMamanger->getModel("../assets/models/Cube/cube.obj");
	Shader *devShader = resourcesMamanger->getShader("dev");
	glm::vec3 devCubeScale = glm::vec3(0.5f, 0.5f, 0.5f);

    is_runing = true;

    while (is_runing) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				is_runing = false;
			else if (event.type == SDL_WINDOWEVENT &&
				event.window.event == SDL_WINDOWEVENT_CLOSE &&
				event.window.windowID == SDL_GetWindowID(window))
				is_runing = false;
            else {
				if(!on_event(event)) {
					Backend::ProcessEvent(rmlContext, event);
				}
			}
		}

		// Update GUI
		for(int i = 0; i < this->guiElements->size(); i += 1) {
			this->guiElements->at(i)->update();
		}
		rmlContext->Update();

		std::vector<Renderable> renderables;
		std::vector<PointLight*> pointLights;
		std::vector<SpotLight*> spotLights;
		DirectionalLight *directionalLight = nullptr;

		// Prepare to rendering frame
		for(int i = 0; i < this->gameObjects->size(); i +=1) {
			GameObject *gObj = this->gameObjects->at(i);

			if(typeid(*gObj) == typeid(DirectionalLightNode)) { // DirectionalLightNode
				if(directionalLight != nullptr) {
					LOG_ERROR("Panic! Multiple directional lights is not allowed.");
					return;
					// TODO: Panic
				}
				DirectionalLightNode *directionalLightNode = static_cast<DirectionalLightNode*>(gObj);
				directionalLight = directionalLightNode->directionalLight;
			} else if(typeid(*gObj) == typeid(BasicGameObject)) { // BasicGameObject
				Model *model = gObj->getModel();
				Renderable renderable = { model, gObj->getShader(), gObj->getPosition(), gObj->getScale(), false };
				renderables.push_back(renderable);
			} else if(typeid(*gObj) == typeid(PointLightNode)) {
				PointLightNode *pointLightNode = static_cast<PointLightNode*>(gObj);
				pointLights.push_back(pointLightNode->pointLight);
				Renderable renderable = { cubeModel, devShader, gObj->getPosition(), devCubeScale };
				renderables.push_back(renderable);
			} else if(typeid(*gObj) == typeid(SpotLightNode)) {
				SpotLightNode *spotLightNode = static_cast<SpotLightNode*>(gObj);
				spotLights.push_back(spotLightNode->spotLight);
				Renderable renderable = { cubeModel, devShader, gObj->getPosition(), devCubeScale };
				renderables.push_back(renderable);
			} else {
				// TODO: Panic
			}
		}

		renderer->renderShadowMap(directionalLight, renderables);

		this->renderer->beginFrame();
		{
			on_render(renderables, directionalLight, pointLights, spotLights);
        	on_render_gui();
		}
        this->renderer->endFrame(props.max_fps, props.is_vsync);
    }

    shutdown();
}

bool Engine::on_event(SDL_Event event) {
	// Camera look
	if(event.type == SDL_MOUSEMOTION) {
		int xrel;
		int yrel;
		SDL_GetRelativeMouseState(&xrel, &yrel);
		
		camera->look_relative((float) xrel, (float) yrel);
	}

	// printf("yaw: %f, pitch: %f", yaw, pitch);

	if(event.type == SDL_KEYUP) {
		if(event.key.keysym.sym == SDLK_w) {
			moveFront = 0;
			return true;
		} else if(event.key.keysym.sym == SDLK_s) {
			moveFront = 0;
			return true;
		} else if(event.key.keysym.sym == SDLK_a) {
			moveRight = 0;
			return true;
		} else if(event.key.keysym.sym == SDLK_d) {
			moveRight = 0;
			return true;
		} 
	}

    if(event.type == SDL_KEYDOWN) {
		if(event.key.keysym.sym == SDLK_F8) { // Reload UI
			for (int i = 0; i < rmlContext->GetNumDocuments(); i++)
			{
				Rml::ElementDocument* document = rmlContext->GetDocument(i);
				const Rml::String& src = document->GetSourceURL();
				if (src.size() > 4 && src.substr(src.size() - 4) == ".rml")
				{
					document->ReloadStyleSheet();
				}
			}
			return true;
		} else if(event.key.keysym.sym == SDLK_w) {
			moveFront = 1;
			return true;
		} else if(event.key.keysym.sym == SDLK_s) {
			moveFront = -1;
			return true;
		} else if(event.key.keysym.sym == SDLK_a) {
			moveRight = -1;
			return true;
		} else if(event.key.keysym.sym == SDLK_d) {
			moveRight = 1;
			return true;
		} else if(event.key.keysym.sym == SDLK_p) {
			if(SDL_GetRelativeMouseMode())
				SDL_SetRelativeMouseMode(SDL_FALSE);
			else
				SDL_SetRelativeMouseMode(SDL_TRUE);

			return true;
		}
	}

	for(int i = 0; i < this->guiElements->size(); i += 1) {
		if(this->guiElements->at(i)->processEvent(event))
			return true;
	}

	return false;
}

void Engine::on_render(
	std::vector<Renderable> renderables,
    DirectionalLight *directionalLight,
    std::vector<PointLight*> pointLights,
    std::vector<SpotLight*> spotLights
) {
	// Update camera is mouse pinned
	if(SDL_GetRelativeMouseMode()) {
		// Camera move
		if(moveFront != 0)
			camera->move_forward(moveFront);
		if(moveRight != 0)
			camera->move_right(moveRight);

		// Update camera
		camera->update();
	}

	renderer->renderFrame(camera, renderables, directionalLight, pointLights, spotLights);
}

void Engine::on_render_gui() {
	Backend::BeginFrame();
	rmlContext->Render();
	Backend::PresentFrame();
}

void Engine::shutdown() {
	// Unload all GUI elements
	for(int i = 0; i < this->guiElements->size(); i += 1) {
		this->guiElements->at(i)->unload();
	}

	Rml::Shutdown();
	Backend::Shutdown();

	SDL_GL_DeleteContext(this->renderer->getGLContext());
	renderer->shutdown();
	renderer = nullptr;
	
	SDL_DestroyWindow(window);
	SDL_Quit();
}