#include "main.hpp"

Engine *engine;
int main(int argc, char ** argv) {
	engine = new Engine();
	engine->init(argc, argv);
	engine->init_gui();
	engine->run();

	return 0;
}