#include "engine.hpp"

// Here is because redefinition error
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

Engine::Engine() {
    // Nothing
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

void Engine::init() {
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
	window = SDL_CreateWindow(ENGINE_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
		window_flags);
	gl_context = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

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
	// ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(ENGINE_GLSL_VERSION);
}

float vertices[] = {
// pos                 // uv
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f,
 1.0f, 0.0f,
0.5f, 0.5f, -0.5f,
 1.0f, 1.0f,
0.5f, 0.5f, -0.5f,
 1.0f, 1.0f,
 -0.5f, 0.5f, -0.5f,
 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,
 0.0f, 0.0f,
-0.5f, -0.5f,
 0.5f,
 0.0f, 0.0f,
0.5f, -0.5f,
 0.5f,
 1.0f, 0.0f,
0.5f, 0.5f,
 0.5f,
 1.0f, 1.0f,
0.5f, 0.5f,
 0.5f,
 1.0f, 1.0f,
-0.5f, 0.5f,
 0.5f,
 0.0f, 1.0f,
-0.5f, -0.5f,
 0.5f,
 0.0f, 0.0f,
-0.5f, 0.5f, 0.5f,
 1.0f, 0.0f,
-0.5f, 0.5f, -0.5f,
 1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
-0.5f, -0.5f, 0.5f,
 0.0f, 0.0f,
-0.5f, 0.5f, 0.5f,
 1.0f, 0.0f,
0.5f, 0.5f, 0.5f,
 1.0f, 0.0f,
0.5f, 0.5f, -0.5f,
 1.0f, 1.0f,
0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
0.5f, -0.5f, 0.5f,
 0.0f, 0.0f,
0.5f, 0.5f, 0.5f,
 1.0f, 0.0f,
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
0.5f, -0.5f, -0.5f,
 1.0f, 1.0f,
0.5f, -0.5f, 0.5f,
 1.0f, 0.0f,
0.5f, -0.5f, 0.5f,
 1.0f, 0.0f,
-0.5f, -0.5f, 0.5f,
 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
-0.5f,
 0.5f, -0.5f,
 0.0f, 1.0f,
0.5f,
 0.5f, -0.5f,
 1.0f, 1.0f,
0.5f,
 0.5f, 0.5f,
 1.0f, 0.0f,
0.5f,
 0.5f, 0.5f,
 1.0f, 0.0f,
-0.5f,
 0.5f, 0.5f,
 0.0f, 0.0f,
-0.5f,
 0.5f, -0.5f,
 0.0f, 1.0f
};

Shader* defaultShader;
Shader* texturedShader;
GLuint VAO;
GLuint VBO;
//GLuint EBO;
unsigned int defaultTexture;

void bind_gl() {
	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(VBO);
}

void load_assets() {
	defaultShader = Shader::load("default");
	texturedShader = Shader::load("textured");

	// Image configs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Load image
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../assets/images/peace.bmp", &width, &height, &nrChannels, 0);

	// TODO: detect errors
	
	// Make GL texture
	glGenTextures(1, &defaultTexture);
	glBindTexture(GL_TEXTURE_2D, defaultTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free
	stbi_image_free(data);
}

glm::mat4 proj;
glm::mat4 model;
glm::mat4 view;
glm::mat4 trans;

glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f, 0.0f, 0.0f),
	glm::vec3( 2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f, 2.0f, -2.5f),
	glm::vec3( 1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

void begin_run() {
	proj = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	view = glm::mat4(1.0f);
	// NOTE: that we’re translating the scene in the reverse direction
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
}

void Engine::run() {
	load_assets();
	bind_gl();
	begin_run();

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
    }

    shutdown();
}

void Engine::on_event(SDL_Event *event) {
    // todo
}

float offset = 0;
void Engine::on_render() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(clear_color.x * clear_color.w,
			clear_color.y * clear_color.w, clear_color.z * clear_color.w,
			clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	texturedShader->use();

	offset += 0.09;

	glBindVertexArray(VAO);
	for(unsigned int i = 0; i < 10; i++) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cubePositions[i]);
	float angle = 20.0f * offset;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

	unsigned int projectionLoc = glGetUniformLocation(texturedShader->program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(proj));
	unsigned int modelLoc = glGetUniformLocation(texturedShader->program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int viewLoc = glGetUniformLocation(texturedShader->program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Draw
	glBindTexture(GL_TEXTURE_2D, defaultTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// use indicies: glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

void Engine::on_render_gui() {
    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();
    {
		ImGui::Begin("Debug");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}