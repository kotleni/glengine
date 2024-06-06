#ifndef H_ENGINE
#define H_ENGINE
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// clang-format off
#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
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

#include "define.hpp"
#include "shader.hpp"
#include "engine_props.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "directional_light.hpp"
#include "skybox.hpp"
#include "resources_manager.hpp"
#include "game_object.hpp"

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

    std::vector<GameObject*> *gameObjects;
public:
    EngineProps props;

    DirectionalLight *directionalLight;

    ResourcesManager *resourcesMamanger;

    Engine();

    void init(int argc, char ** argv);
    void init_gui();
    void render_splash();
    void run();
    void shutdown();

    glm::vec2 get_render_size();
};

static Engine *instance;
static Engine *engine() {
    return instance;
}

#endif