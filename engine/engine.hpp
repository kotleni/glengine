#ifndef H_ENGINE
#define H_ENGINE
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

// clang-format off
#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// clang-format on

// clang-format off
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
// clang-format on

// clang-format off
#include <RmlUi/Core.h>
#include <RmlUi_Backend.h>
// clang-format on

#include "define.hpp"
#include "shader.hpp"
#include "engine_props.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "directional_light.hpp"
#include <skybox.hpp>
#include "resources_manager.hpp"
#include "game_object.hpp"
#include <renderer.hpp>

#include "gui/baseguielement.h"
#include "gui/consoleguielement.h"

class Engine {
private:
    void on_event(SDL_Event *event); 
    void on_render();
    void on_render_gui();

    SDL_Window *window;
    Renderer *renderer;
    Rml::Context* rmlContext;
    bool is_runing;

    std::vector<GameObject*> *gameObjects;
    std::vector<BaseGuiElement*> *guiElements;
public:
    EngineProps props;
    ResourcesManager *resourcesMamanger;

    Engine();

    void init(int argc, char ** argv);
    void init_gui();
    void render_splash();
    void run();
    void shutdown();
};

static Engine *instance;
static Engine *engine() {
    return instance;
}

#endif