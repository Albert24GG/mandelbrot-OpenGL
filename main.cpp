#include <iostream>
#include <fstream>
#include <sstream>
#include <format>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "helpers.h"


// Set default WIDTH and HEIGHT values
constexpr GLint WIDTH = 800, HEIGHT = 600;

// Set the paths to the shaders
const char* VERTEX_SHADER_PATH = "./shaders/vertex_shader.glsl";
const char* FRAGMENT_SHADER_PATH = "./shaders/fragment_shader.glsl";


int main(int argc, char** argv) {
	
	// -------------------------------- INIT ------------------------------- //
	

	// Initialize GLFW
	
	if (glfwInit() != GL_TRUE) {
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}
	
	// Set OpenGL 4.6 context and core profile

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Only for mac 
#ifdef __APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot zoom", nullptr, nullptr);

	// Set window aspect ratio
	//glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	// Set the callback necessary for the window
	setWindowCallbacks(window);

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
	

	bool isPanning = false;
	double xPrevPos = 0.0, yPrevPos = 0.0;
	
	// Render loop. Keep the window up until it is closed

	while (!glfwWindowShouldClose(window)) {

		// Draw the shape

		shaderProgram.use();

		// Pass window width and height to the shader
		glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
		shaderProgram.bindValues(currentWidth, currentHeight, glfwGetTime(), off.x , off.y, zoom, maxIterations);

		// Get current cursor position
		double xCurrentPos, yCurrentPos;
		getMouseCoordinates(window, xCurrentPos, yCurrentPos);

		// Update titlebar information
		glfwSetWindowTitle(window, std::format("Mandelbrot zoom | Mouse location: x={} y={} | Zoom={} | Iteration count={}", (double)xCurrentPos, (double)yCurrentPos, zoom, maxIterations).c_str());

		if (isPanning) {
			// Change the offset position according to the mouse movement
			off.x -= (xCurrentPos - xPrevPos);
			off.y -= (yCurrentPos - yPrevPos);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
			isPanning = true;
			
			// Remember the position of the mouse when starting panning
			xPrevPos = xCurrentPos;
			yPrevPos = yCurrentPos;
		}
		// If left click is released, stop panning
		isPanning = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_RELEASE);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, (GLvoid*) 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Delete all GLFW resources allocated

	glfwTerminate();
	return 0;
}