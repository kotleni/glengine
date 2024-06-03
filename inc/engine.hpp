#include <iostream>
#include <memory>
#include <string>
#include <vector>

// clang-format off
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
// clang-format on

// clang-format off
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"
// clang-format on

// clang-format off
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
// clang-format on

#define ENGINE_NAME "engine"
#define ENGINE_GLSL_VERSION "#version 130"

class Engine {
private:
    void on_event(SDL_Event *event); 
    void on_render();
    void on_render_gui();

    SDL_Window *window;
    SDL_GLContext gl_context;
    ImGuiIO io;
    bool is_runing;
    ImVec4 clear_color;
public:
    Engine();

    void init();
    void init_gui();
    void run();
    void shutdown();
};