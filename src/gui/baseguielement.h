#ifndef H_BASE_GUI_ELEMENT
#define H_BASE_GUI_ELEMENT

#include <RmlUi/Core.h>
#include <iostream>

class BaseGuiElement {
public:
    virtual void load(Rml::Context* rmlContext) = 0;
    virtual void update() = 0;
    virtual void unload() = 0;
};

#define DEFINE_GUI_ELEMENT(CLASS_NAME) class CLASS_NAME : public BaseGuiElement { \
    void load(Rml::Context* rmlContext); \
    void update(); \
    void unload();

#define END_OF_DEFINE_GUI_ELEMENT };

#endif