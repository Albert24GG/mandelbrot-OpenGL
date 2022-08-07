#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setScreenColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void setColor(const GLuint &shaderProgram);


// Set default WIDTH and HEIGHT values
constexpr GLint WIDTH = 800, HEIGHT = 600;
const char* VERTEX_SHADER_PATH = "./shaders/vertex_shader.glsl";
const char* FRAGMENT_SHADER_PATH = "./shaders/fragment_shader.glsl";


int main(int argc, char** argv) {
	// -------------------------------- INIT ------------------------------- //
	

	// Initialize GLFW
	if (glfwInit() != GL_TRUE) {
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}
	
	// Set OpenGL 3.3 context and core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Only for mac 
#ifdef __APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE_

    // Create a window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGl", nullptr, nullptr);

	// Set window aspect ratio
	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Set viewport every time the window is resized

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}
	

	// -------------------------------- SHADERS ------------------------------- //

	Shader shaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	
	// -------------------------------- VERTEX DATA ------------------------------- //

	GLfloat vertices[] = {
		 -1.0f,  1.0f, 0.0f, // top left
		 -1.0f, -1.0f, 0.0f, // bottom left
		  1.0f, -1.0f, 0.0f, // bottom right
		  1.0f,  1.0f, 0.0f  // top right
	};

	GLuint indices[] = {
		0, 1, 2,  // first triangle
		2, 3, 0   // second triangle
	};

	// Generate a Vertex Array Object and bind it to the vertex array buffer

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	

	// Generate a Vertex Buffer Object and bind it to the array buffer
	
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy the data from vertices to the Array Buffer(assigned to the VBO)
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Generate an EBO and bind in to the element buffer object
	
	GLuint EBO;
	glGenBuffers(1, &EBO);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Configure vertex attributes

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	// Unbind

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// -------------------------------- RENDERING ------------------------------- //
	

	// Render loop. Keep the window up until it is closed

	while (!glfwWindowShouldClose(window)) {
		
		setScreenColor(39.0f / 255.0f, 138.0f / 255.0f, 245.0f / 255.0f, 0.8f);

		// process the input
		processInput(window);

		// Draw the shape

		shaderProgram.use();

		// Pass window width and height to the shader
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		shaderProgram.bindValues(width, height, glfwGetTime());
		//std::cout << width << ' ' << height << '\n';

		// Set the color for a shader
		setColor(shaderProgram.getID());
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, (GLvoid*) 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Delete all GLFW resources allocated

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	// Listen for Esc and close window when key pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


void setScreenColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}




void setColor(const GLuint &shaderProgram) {
	double timeVal = glfwGetTime();
	float greenVal = abs(sin(timeVal)/2);
	GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "color");

	glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation, 39.0f / 255.0f, greenVal, 87.0f / 255.0f, 0.8);
}
