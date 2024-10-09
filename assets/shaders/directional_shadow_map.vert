#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;

uniform mat4 model;
uniform mat4 directionalLightTransform;

void main()
{
	gl_Position = directionalLightTransform * model * vec4(pos, 1.0);
}