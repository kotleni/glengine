// Detect compiling target bit
#if ((ULONG_MAX) == (UINT_MAX))
    #define OS_32BIT
#else
    #define OS_64BIT
#endif

// Engine defenitions
#define ENGINE_NAME "engine"
#define ENGINE_GLSL_VERSION "#version 130"