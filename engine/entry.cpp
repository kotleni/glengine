#include "engine.hpp"

#if defined(WINDOWS)
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(LINUX) || defined(OSX)
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    #error Unknown dynamic link import/export semantics.
#endif

// Exporting function to calling from launcher (ak executable binary)
extern "C" {
    EXPORT int engineMain(int argc, char ** argv) {
	    Engine *engine = new Engine();
	    engine->init(argc, argv);
	    engine->init_gui();
	    engine->run();

	    return 0;
    }
}