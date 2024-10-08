#ifndef H_SHADER
#define H_SHADER

#include <GL/glew.h>

#if defined(LINUX) | defined(WINDOWS)
#include <GL/gl.h>
#elif defined(OSX)
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#endif
#include <SDL2/SDL_opengl.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#define MAX_SPOT_LIGHTS 32
#define MAX_POINT_LIGHTS 32

#include <logger.hpp>

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

class Shader {
public:
    GLuint program;

    int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess,
		uniformTexture, uniformDirectionalShadowMap,
		uniformDirectionalLightTransform;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

    Shader(GLint programId);
    void use();

    GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColourLocation();
	GLuint getDiffuseIntensityLocation();
	GLuint getDirectionLocation();
	GLuint getSpecularIntensityLocation();
	GLuint getShininessLocation();
	GLuint getEyePositionLocation();

    void setDirectionalLight(DirectionalLight * dLight);
	void setPointLights(PointLight * pLight, unsigned int lightCount);
	void setSpotLights(SpotLight * sLight, unsigned int lightCount);
	void setTexture(GLuint textureUnit);
	void setDirectionalShadowMap(GLuint textureUnit);
	void setDirectionalLightTransform(glm::mat4* lTransform);

    static Shader *load(std::string file);
};

#endif