#include "main.hpp"

int main(int argc, char ** argv) {
	Engine *engine = new Engine();
	engine->init(argc, argv);
	engine->init_gui();
	engine->run();

	return 0;
}