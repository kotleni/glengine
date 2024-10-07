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
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		LOG_INFO("{}\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

    Shader *shader = new Shader(ProgramID);
    return shader;
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::setMat4(const char* key, glm::mat4 mat) {
	unsigned int loc = glGetUniformLocation(program, key);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const char* key, glm::vec3 vec) {
	unsigned int loc = glGetUniformLocation(program, key);
	glUniform3fv(loc, 1, glm::value_ptr(vec));
}

void Shader::setVec2(const char* key, glm::vec2 vec) {
	unsigned int loc = glGetUniformLocation(program, key);
	glUniform2fv(loc, 1, glm::value_ptr(vec));
}

void Shader::setFloat(const char* key, float value) {
	unsigned int loc = glGetUniformLocation(program, key);
	glUniform1f(loc, value);
}

void Shader::setBool(const char* key, bool value) {
	unsigned int loc = glGetUniformLocation(program, key);
	glUniform1i(loc, value);
}

void Shader::setInt(const char* key, int value) {
	unsigned int loc = glGetUniformLocation(program, key);
	glUniform1i(loc, value);
}