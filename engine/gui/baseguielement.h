#ifndef H_BASE_GUI_ELEMENT
#define H_BASE_GUI_ELEMENT

#include <SDL2/SDL.h>
#include <RmlUi/Core.h>
#include <iostream>

class BaseGuiElement {
public:
    virtual void load(Rml::Context* rmlContext) = 0;
    virtual void update() = 0;
    virtual void unload() = 0;
    virtual bool processEvent(SDL_Event event) = 0;
};

#define DEFINE_GUI_ELEMENT(CLASS_NAME) class CLASS_NAME : public BaseGuiElement { \
    void load(Rml::Context* rmlContext); \
    void update(); \
    void unload(); \
    bool processEvent(SDL_Event event);

#define END_OF_DEFINE_GUI_ELEMENT };

#endif