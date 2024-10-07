#include "consoleguielement.h"

void ConsoleGuiElement::load(Rml::Context* rmlContext) {
	this->elementDocument = rmlContext->LoadDocument("../assets/ui/layouts/console.rml");
	if (!elementDocument) {
        std::cout << "ConsoleGuiElement::load is failed. LoadDocument return false." << std::endl;
        return;
    }
}

void ConsoleGuiElement::update() {
    if(!this->elementDocument) return;
    
    Rml::Element *textElement = elementDocument->GetElementById("text");
	textElement->SetInnerRML("<b>[Console]</b> Initialized as view element.");
    // TODO: Cleanup here is needed? delete textElement;
}

bool ConsoleGuiElement::processEvent(SDL_Event event) {
    if(event.type != SDL_KEYUP) return false;

    switch(event.key.keysym.sym) {
        case SDLK_F1:
            if(elementDocument->IsVisible())
                elementDocument->Hide();
            else
                elementDocument->Show();

            return true;

        case SDLK_RETURN:
            // TODO: Check is in focus
            if(elementDocument->IsVisible()) {
                Rml::Element *textField = elementDocument->GetElementById("input");
                Rml::String string = textField->GetAttribute<Rml::String>("value", "");
                std::cout << string << std::endl;
            }
            return true;
    }

    return false;
}

void ConsoleGuiElement::unload() {
    // TODO: Cleanup here is needed? delete elementDocument;
}