#include <iostream>
#include <memory>
#include <string>
#include <vector>

#if defined(LINUX) || defined(OSX)
	#include <dlfcn.h>
#elif defined(WINDOWS)
	#error Windows is not supported yet.
#endif

typedef int (*engineMain)( int argc, char **argv);

int main(int argc, char ** argv) {
	#if defined(LINUX)
		const char *engineLibPath = "bin/libengine.so";
	#elif defined(OSX)
		const char *engineLibPath = "bin/libengine.dylib";
	#elif defined(WINDOWS)
		const char *engineLibPath = "bin/engine.dll";
	#endif

	#if defined(LINUX) || defined(OSX)
		void *engine = dlopen(engineLibPath, RTLD_NOW);

		if (!engine) {
			printf( "Failed to load the engine(%s) (%s)\n", engineLibPath, dlerror() );
			while(1);
			return 0;
		}

		engineMain main = (engineMain)dlsym(engine, "engineMain");
		if(!main) {
			printf("Failed to load the engine entry proc\n");
			while(1);
			return 0;
		}

		return main( argc, argv );
	#elif defined(WINDOWS)
		#error Windows is not supported yet.
	#endif

	return 0;
}