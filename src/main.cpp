#include "main.hpp"

Engine *engine;
int main(int argc, char ** args) {
	engine = new Engine();
	engine->init();
	engine->init_gui();
	engine->run();

	return 0;
}