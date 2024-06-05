#include "engine.hpp"

// NOTE: this header is header only lib
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// NOTE: this header is header only lib
#include "args.hpp"

int moveFront = 0;
int moveRight = 0;

Shader* texturedShader;
Shader* skyboxShader;
GLuint VAO;
GLuint VBO;
unsigned int skyboxVAO, skyboxVBO;
//GLuint EBO;

Model *castleModel;
Camera *camera;

float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

Engine::Engine() {
	instance = this;
    directionalLight = new DirectionalLight(glm::vec3(1.0f, -1.0f, -0.3f));
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
    args::Flag tools(parser, "tools", "Disable ticks and enable developer tools in game.", {"tools"});
    args::Flag vsync(parser, "vsync", "Enable V-Sync in render loop.", {"vsync"});

	std::vector<std::string>::const_iterator args = parser.ParseArgs(str_args);
	
	// Init props default values
	props.is_vsync = bool{vsync};
	props.max_fps = 200;
	props.is_tools_mode = bool{tools};
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
	
	#ifdef OS_MACOS
		SDL_GL_SetAttribute(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
	#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	glm::vec2 render_size = this->get_render_size();
	window = SDL_CreateWindow(ENGINE_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, render_size.x, render_size.y,
		window_flags);
	gl_context = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, gl_context);

	if(props.is_vsync)
		SDL_GL_SetSwapInterval(1); // Enable vsync

	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Glew
	glewExperimental = GL_TRUE; 
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable              ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( MessageCallback, 0 );
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
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
	// ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(ENGINE_GLSL_VERSION);
}

void load_assets() {
	texturedShader = Shader::load("textured");
	skyboxShader = Shader::load("skybox");
	castleModel = new Model("../assets/models/Castle OBJ.obj");
	camera = new Camera();
}

unsigned int cubemapTexture;

void init_skybox() {
	glGenTextures(1, &cubemapTexture);

	std::vector<std::string> textures_faces = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	int width, height, nrChannels;
	unsigned char *data;
	for(unsigned int i = 0; i < textures_faces.size(); i++) {
		std::string path = "../assets/images/" + textures_faces[i];
		data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// init skybox values
	glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Engine::run() {
	load_assets();
	init_skybox();

    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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

        on_render();
        on_render_gui();

        SDL_GL_SwapWindow(window);

		// FPS cap
		if(!props.is_vsync) {
			SDL_Delay(1000 / props.max_fps);
		}
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
	// Camera move
	if(moveFront != 0)
		camera->move_forward(moveFront);
	if(moveRight != 0)
		camera->move_right(moveRight);

	// Update camera
	camera->update();

    glViewport(0, 0, this->get_render_size().x, this->get_render_size().y);
	glClearColor(clear_color.x * clear_color.w,
			clear_color.y * clear_color.w, clear_color.z * clear_color.w,
			clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthMask(GL_FALSE);

	skyboxShader->use();
	glm::mat4 view = glm::mat4(glm::mat3(camera->get_view()));
	skyboxShader->setInt("skybox", 1);
	skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", camera->get_projection());

	glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	// Draw objects
	texturedShader->use();

	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

	// TODO: split logic to shader and material
	// Bind model matrix
	texturedShader->setMat4("model", model);

	// Material bind
	texturedShader->setVec3("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
	texturedShader->setVec3("material.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
	texturedShader->setVec3("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
	texturedShader->setFloat("material.shininess", 76.8f);

	texturedShader->setBool("features.is_use_light", props.is_render_light);

	// Camera bind
	camera->applyToShader(texturedShader);

	// Light bind
	directionalLight->apply(texturedShader);

	// Draw
	castleModel->Draw(*texturedShader);
}

void Engine::on_render_gui() {
    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();

	if(props.is_tools_mode) {
		ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderInt("Max fps", &props.max_fps, 15, 200);
	ImGui::Checkbox("Light", &props.is_render_light);
	ImGui::Checkbox("Vsync", &props.is_vsync);
	ImGui::End();

	// Rendering
	ImGui::Render();
    auto raw = ImGui::GetDrawData();
    ImGui_ImplOpenGL3_RenderDrawData(raw);

	SDL_GL_SetSwapInterval(props.is_vsync);
}

void Engine::shutdown() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

glm::vec2 Engine::get_render_size() {
	return glm::vec2(1600, 800);
}