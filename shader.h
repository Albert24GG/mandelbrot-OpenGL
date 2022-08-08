#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
private:
	
	GLuint* ID = new GLuint;

	// Utility funtion that loads the shader source code and creates a shader

	void loadShader(const char* shaderPath, const GLenum& shaderType, GLuint& shader);

	// Function that reads the source code and returns a string

	const std::string readFileToString(const char* path);

public:
	
	// Constructor that reads and builds the shader program

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	
	// Destructor

	~Shader();
	
	// Retrive the ID of the shader program

	GLuint getID();

	// Bind all the CPU values to the GPU values

	void bindValues(const GLuint& width, const GLuint& height, const GLfloat& time, const double& x, const double& y, const double& zoom);

	// Activate the shader program;
	
	void use();

};