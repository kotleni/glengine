// Detect compiling target bit
#if ((ULONG_MAX) == (UINT_MAX))
    #define OS_32BIT
#else
    #define OS_64BIT
#endif

// Detect OS
#if defined(_WIN32) || defined(__CYGWIN__)
    #define OS_WINDOWS
#elif defined(__linux__)
    #define OS_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
    #define OS_MACOS
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #define OS_UNIX
#else
    #error Unknown environment!
#endif

// Engine defenitions
#define ENGINE_NAME "engine"
#define ENGINE_GLSL_VERSION "#version 330"