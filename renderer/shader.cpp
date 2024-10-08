#include "shader.hpp"

Shader::Shader(GLint programId) {
    program = programId;
}

Shader *Shader::load(std::string file) {
    std::string vert_path = "../assets/shaders/";
    vert_path.append(file);
    vert_path.append(".vert");

    std::string frag_path = "../assets/shaders/";
    frag_path.append(file);
    frag_path.append(".frag");

    // Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vert_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Can't to open {}.\n", vert_path.c_str());
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(frag_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	LOG_INFO("Compiling shader : {}", vert_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("{}\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	LOG_INFO("Compiling shader : {}", frag_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("{}\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	LOG_INFO("Linking program");
	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, VertexShaderID);
	glAttachShader(shaderID, FragmentShaderID);
	glLinkProgram(shaderID);

	// Check the program
	glGetProgramiv(shaderID, GL_LINK_STATUS, &Result);
	glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(shaderID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		LOG_INFO("{}\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(shaderID, VertexShaderID);
	glDetachShader(shaderID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

    Shader *shader = new Shader(shaderID);

	shader->uniformProjection = glGetUniformLocation(shaderID, "projection");
	shader->uniformModel = glGetUniformLocation(shaderID, "model");
	shader->uniformView = glGetUniformLocation(shaderID, "view");
	shader->uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	shader->uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	shader->uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	shader->uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	shader->uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	shader->uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	shader->uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	shader->uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		shader->uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		shader->uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		shader->uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		shader->uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		shader->uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		shader->uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		shader->uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	shader->uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		shader->uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		shader->uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		shader->uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		shader->uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		shader->uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		shader->uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		shader->uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		shader->uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		shader->uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	shader->uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	shader->uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	shader->uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

    return shader;
}

GLuint Shader::getProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::getModelLocation()
{
	return uniformModel;
}
GLuint Shader::getViewLocation()
{
	return uniformView;
}
GLuint Shader::getAmbientColourLocation()
{
	return uniformDirectionalLight.uniformColour;
}
GLuint Shader::getAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}
GLuint Shader::getDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}
GLuint Shader::getDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}
GLuint Shader::getSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}
GLuint Shader::getShininessLocation()
{
	return uniformShininess;
}
GLuint Shader::getEyePositionLocation()
{
	return uniformEyePosition;
}

void Shader::setDirectionalLight(DirectionalLight * dLight)
{
	dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight * pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

void Shader::setSpotLights(SpotLight * sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);
	}
}

void Shader::setTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::use() {
    glUseProgram(program);
}