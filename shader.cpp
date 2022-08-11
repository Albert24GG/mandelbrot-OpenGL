#include "shader.h"


const std::string Shader::readFileToString(const char* path) {
	std::ifstream in;

	// Ensure ifstream object can throw exceptions
	
	in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		
		in.open(path, std::ios::in | std::ios::binary);
		
		std::stringstream content;
		
		content << in.rdbuf();
		
		in.close();
		
		return content.str();
	}
	catch (std::ifstream::failure err) {
		
		std::cout << "ERROR:SHADER_SOURCE_COULD_NOT_BE_READ\n";
	
	}
}


void Shader::loadShader(const char* shaderPath, const GLenum& shaderType, GLuint& shader) {
	// Read the shader source as std::string and convert it to GLchar*
	const std::string& tempSource = readFileToString(shaderPath);  // lvalue reference to the const string returned to extend lifetime of the string
	const GLchar* shaderSource = (GLchar*)(tempSource.c_str());

	// Create a shader object
	shader = glCreateShader(shaderType);

	// Attach the shader source to the shader object and compile
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	// Verify if compilation was successfull
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << '\n';
	}
}


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	// Vertex Shader

	GLuint vertexShader;
	loadShader(vertexShaderPath, GL_VERTEX_SHADER, vertexShader);
	
	// Fragment Shader

	GLuint fragmentShader;
	loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER, fragmentShader);

	// Create the Shader Program and link the vertex and fragment shader to it

	*ID = glCreateProgram();
	glAttachShader(*ID, vertexShader);
	glAttachShader(*ID, fragmentShader);
	glLinkProgram(*ID);

	// Cleanup

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Check for linking errors
	GLint success;
	glGetProgramiv(*ID, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(*ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}
}


Shader::~Shader() {
	// Delete the shader program

	glDeleteProgram(*ID);

	delete ID;
}


void Shader::use() {
	glUseProgram(*this->ID);
}


GLuint Shader::getID() {
	return *this->ID;
}

void Shader::bindValues(const GLuint& width, const GLuint& height, const GLfloat& time, const double& x, const double& y, const double& zoom) {
	// Ensure that the correct shader program is in use
	
	this->use();
	
	// Pass window resolution, time, offset and zoom to the shader
	glUniform2ui(glGetUniformLocation(*this->ID, "windowResolution"), width, height);
	glUniform1f(glGetUniformLocation(*this->ID, "time"), time);
	glUniform2d(glGetUniformLocation(*this->ID, "off"), x, y);
	glUniform1d(glGetUniformLocation(*this->ID, "zoom"), zoom);
}