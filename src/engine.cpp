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

Shader* texturedShader;

Camera *camera;
Skybox *skybox;

Engine::Engine() {
	instance = this;
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL: %s\n", message );
}

void Engine::init(int argc, char ** argv) {
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
		props.is_tools_mode = bool{tools};
	#endif
	props.is_render_light = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
		0) {
		printf("Error: %s\n", SDL_GetError());
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
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
	);
	glm::vec2 window_size = glm::vec2(1200, 800);
	window = SDL_CreateWindow(ENGINE_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size.x, window_size.y,
		window_flags);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, gl_context);

	this->renderer = new Renderer(window, gl_context);

	// Glew
	glewExperimental = GL_TRUE; 
	glewInit();

	glEnable(GL_DEPTH_TEST);
	#ifndef OS_MACOS
		// INFO: macOS don't support debug opengl
		glEnable              ( GL_DEBUG_OUTPUT );
		glDebugMessageCallback( MessageCallback, 0 );
	#endif
}

void Engine::init_gui() {
    // Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
	// Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
	// Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	
	// FIXME: engine has big problems with HighDPI rendering
	// FIXME: Temporary hack for macOS retina
	#ifdef OS_MACOS
		ImGui::GetStyle().ScaleAllSizes(0.5);
	#endif
	
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, this->renderer->getGLContext());
	ImGui_ImplOpenGL3_Init(ENGINE_GLSL_VERSION);

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Engine::render_splash() {
	// NOTE: Double render, not shown at first
	for(int i = 0; i < 2; i += 1) {
		this->renderer->beginFrame();

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
		}

		ImGui_ImplOpenGL3_NewFrame();
		{
			ImGui_ImplSDL2_NewFrame();

			ImGui::NewFrame();

			ImGui::Begin("Engine", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::Text("Loading...");
				ImGui::ProgressBar(100, ImVec2(200, 0));
			ImGui::End();

			// Rendering
			ImGui::Render();
    		auto raw = ImGui::GetDrawData();
    		ImGui_ImplOpenGL3_RenderDrawData(raw);
		}

		this->renderer->endFrame();
	}
}

void Engine::run() {
	render_splash();

	SDL_SetRelativeMouseMode(SDL_TRUE);

	this->gameObjects = new std::vector<GameObject*>();

	resourcesMamanger = new ResourcesManager();
	resourcesMamanger->loadAll();

	texturedShader = resourcesMamanger->getShader("textured");

	GameObject *castleObj = new GameObject(resourcesMamanger, "../assets/models/Castle/castle.obj");
	gameObjects->push_back(castleObj);

	glm::vec2 render_size = engine()->renderer->get_render_size();
	camera = new Camera(render_size);

	skybox = new Skybox();
	skybox->load("skybox1");

    is_runing = true;

    while (is_runing) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				is_runing = false;
			else if (event.type == SDL_WINDOWEVENT &&
				event.window.event == SDL_WINDOWEVENT_CLOSE &&
				event.window.windowID == SDL_GetWindowID(window))
				is_runing = false;
            else
                on_event(&event);
		}

		this->renderer->beginFrame();
		{
			on_render();
        	on_render_gui();
		}
        this->renderer->endFrame(props.max_fps, props.is_vsync);
    }

    shutdown();
}

void Engine::on_event(SDL_Event *event) {
	// Camera look
	if(event->type == SDL_MOUSEMOTION) {
		int xrel;
		int yrel;
		SDL_GetRelativeMouseState(&xrel, &yrel);
		
		camera->look_relative((float) xrel, (float) yrel);
	}

	// printf("yaw: %f, pitch: %f", yaw, pitch);

	if(event->type == SDL_KEYUP) {
		if(event->key.keysym.sym == SDLK_w) {
			moveFront = 0;
		} else if(event->key.keysym.sym == SDLK_s) {
			moveFront = 0;
		} else if(event->key.keysym.sym == SDLK_a) {
			moveRight = 0;
		} else if(event->key.keysym.sym == SDLK_d) {
			moveRight = 0;
		} 
	}

    if(event->type == SDL_KEYDOWN) {
		if(event->key.keysym.sym == SDLK_w) {
			moveFront = 1;
		} else if(event->key.keysym.sym == SDLK_s) {
			moveFront = -1;
		} else if(event->key.keysym.sym == SDLK_a) {
			moveRight = -1;
		} else if(event->key.keysym.sym == SDLK_d) {
			moveRight = 1;
		} else if(event->key.keysym.sym == SDLK_p) {
			if(SDL_GetRelativeMouseMode())
				SDL_SetRelativeMouseMode(SDL_FALSE);
			else
				SDL_SetRelativeMouseMode(SDL_TRUE);
		}
	}
}


void Engine::on_render() {
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

	// Draw skybox
	skybox->draw(camera);

 	texturedShader->use();

	// Props bind
	texturedShader->setBool("features.is_use_light", props.is_render_light);

	// Camera bind
	camera->applyToShader(texturedShader);

	// Light bind
	directionalLight->apply(texturedShader);

	// Draw
	for(int i = 0; i < this->gameObjects->size(); i +=1) {
		GameObject *gObj = this->gameObjects->at(i);
		gObj->draw(camera, texturedShader);
	}
}

int selectedIndex = -1;
void Engine::on_render_gui() {
	const int v_docks_width = 260;

    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();

	// TODO: allow toggle bar in non tool mode
	if(props.is_tools_mode) {
		auto io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - v_docks_width, 0));
		ImGui::SetNextWindowSize(ImVec2(v_docks_width, io.DisplaySize.y));
		
		ImGui::Begin("Misc", nullptr,
			ImGuiWindowFlags_MenuBar | 
			ImGuiWindowFlags_AlwaysAutoResize | 
			ImGuiWindowFlags_NoBringToFrontOnFocus | 
            ImGuiWindowFlags_NoNavFocus |                                        
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse
			);

		ImGui::DockSpace(ImGui::GetID("Dockspace1"), ImVec2(0.0f, 0.0f),  ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::BeginTabBar("Misc");
			if(ImGui::BeginTabItem("Settings")) {
				ImGui::SliderInt("FPS", &props.max_fps, 15, 180);
				ImGui::Checkbox("Light", &props.is_render_light);
				ImGui::Checkbox("Vsync", &props.is_vsync);
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Debug")) {
				ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
				ImGui::Text("DPI Scale: %.1f", ImGui::GetPlatformIO().Monitors[0].DpiScale);
				ImGui::EndTabItem();
			}
		ImGui::EndTabBar();

		ImGui::End();
	}

	if(props.is_tools_mode) {
		auto io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(v_docks_width, io.DisplaySize.y));

		ImGui::Begin("Game objects", nullptr,
			ImGuiWindowFlags_MenuBar | 
			ImGuiWindowFlags_AlwaysAutoResize | 
			ImGuiWindowFlags_NoBringToFrontOnFocus | 
            ImGuiWindowFlags_NoNavFocus |                                        
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse
			);
			  
		ImGui::DockSpace(ImGui::GetID("Dockspace2"), ImVec2(0.0f, 0.0f),  ImGuiDockNodeFlags_PassthruCentralNode);

		if (ImGui::BeginTabBar("Tab1")) {
			if(ImGui::BeginTabItem("Objects")) {
				if(this->gameObjects->size() > 0) {
			for(int i = 0; i < this->gameObjects->size(); i +=1) {
				GameObject *gObj = this->gameObjects->at(i);

				// Selector
				if(selectedIndex == i) {
					ImGui::Text("*");
					ImGui::SameLine();
				}

				// Name
				std::filesystem::path path(gObj->name);
				ImGui::Text("%s", path.filename().c_str());

				// Select btn
				if(selectedIndex != i) {
					ImGui::SameLine();
					if(ImGui::Button("Select"))
						selectedIndex = i;
				}
			}
		} else {
			ImGui::Text("Empty list");
		}
		ImGui::EndTabItem();
			}
                if (ImGui::BeginTabItem("Editor")) {
					if(props.is_tools_mode && selectedIndex != -1) {
		GameObject *gObj = this->gameObjects->at(selectedIndex);

		float *position = new float[] { gObj->position.x, gObj->position.y, gObj->position.z };
		float *scale = new float[] { gObj->scale.x, gObj->scale.y, gObj->scale.z };

		//ImGui::Begin(gObj->name.c_str(), nullptr, ImGuiWindowFlags_DockNodeHost);
		if(ImGui::InputFloat3("Position", position)) {
			gObj->position.x = position[0];
			gObj->position.y = position[1];
			gObj->position.z = position[2];
		}
		// TODO: impl rotation ImGui::InputFloat3("Rotation", arr);
		if(ImGui::InputFloat3("Scale", scale)) {
			gObj->scale.x = scale[0];
			gObj->scale.y = scale[1];
			gObj->scale.z = scale[2];
		}
		if(ImGui::Button("Remove")) {
			this->gameObjects->erase(this->gameObjects->begin() + selectedIndex);
			selectedIndex = -1;
		}
	}
                    ImGui::EndTabItem();
                }
				
                ImGui::EndTabBar();
            }

		
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
    auto raw = ImGui::GetDrawData();
    ImGui_ImplOpenGL3_RenderDrawData(raw);
}

void Engine::shutdown() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	renderer->shutdown();
	renderer = nullptr;
	SDL_GL_DeleteContext(this->renderer->getGLContext());
	SDL_DestroyWindow(window);
	SDL_Quit();
}