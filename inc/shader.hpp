#include <GL/glew.h>
#include <SDL_opengl.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
public:
    GLuint program;

    Shader(GLint programId);
    void use();

    static Shader *load(char* file);
};