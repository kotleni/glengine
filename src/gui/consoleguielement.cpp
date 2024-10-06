#include "consoleguielement.h"

void ConsoleGuiElement::load(Rml::Context* rmlContext) {
	this->elementDocument = rmlContext->LoadDocument("../assets/ui/layouts/console.rml");
	if (!elementDocument) {
        std::cout << "ConsoleGuiElement::load is failed. LoadDocument return false." << std::endl;
        return;
    }

    elementDocument->Show();
}

void ConsoleGuiElement::update() {
    if(!this->elementDocument) return;
    
    Rml::Element *textElement = elementDocument->GetElementById("text");
	textElement->SetInnerRML("<b>[Console]</b> Initialized as view element.");
    // TODO: Cleanup here is needed? delete textElement;
}

void ConsoleGuiElement::unload() {
    // TODO: Cleanup here is needed? delete elementDocument;
}