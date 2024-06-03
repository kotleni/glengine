#version 330 core

layout(location = 0) in vec3 vertPos;
out vec3 fragPos;

void main() {
    gl_Position.xyz = vertPos;
    gl_Position.w = 1.0;

    fragPos = vertPos;
}