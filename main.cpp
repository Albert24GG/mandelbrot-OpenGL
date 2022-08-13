#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void changeCoordRange(GLFWwindow* window, bool mode);
void normalizeCoord(double& x, double& y);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Set default WIDTH and HEIGHT values
constexpr GLint WIDTH = 800, HEIGHT = 600;

// Set the paths to the shaders
const char* VERTEX_SHADER_PATH = "./shaders/vertex_shader.glsl";
const char* FRAGMENT_SHADER_PATH = "./shaders/fragment_shader.glsl";

struct coord {
	double x, y;
} off{0.0, 0.0};

double zoom = 1.0;
int currentWidth, currentHeight;

int main(int argc, char** argv) {
	// -------------------------------- INIT ------------------------------- //
	

	// Initialize GLFW
	if (glfwInit() != GL_TRUE) {
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}
	
	// Set OpenGL 4.0 context and core profile
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

	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Set viewport every time the window is resized

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Register the cursor position

	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Register the action of zooming
	
	glfwSetScrollCallback(window, scroll_callback);

	// Process every key pressed

	glfwSetKeyCallback(window, key_callback);

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

	bool isPanning = false;
	double xPrevPos = 0.0, yPrevPos = 0.0;

	while (!glfwWindowShouldClose(window)) {

		// Draw the shape

		shaderProgram.use();

		// Pass window width and height to the shader
		glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
		shaderProgram.bindValues(currentWidth, currentHeight, glfwGetTime(), off.x , off.y, zoom);
		//shaderProgram.bindValues(currentWidth, currentHeight, glfwGetTime(), xOff.min, xOff.max, yOff.min, yOff.max);

		// Get current cursor position
		double xCurrentPos, yCurrentPos;
		glfwGetCursorPos(window, &xCurrentPos, &yCurrentPos);
		yCurrentPos = currentHeight - yCurrentPos;
		normalizeCoord(xCurrentPos, yCurrentPos);

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


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		// Move by 1% in all directions
		double lenx = (4.0 * currentWidth / currentHeight) / zoom;
		int signx = -1 * (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) + (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT); // key A / LEFT ARROW pressed -> signx = -1 (moving right);  key D / RIGHT ARROW pressed -> signx = 1; (moving left)
		
		double leny = 4 / zoom;
		int signy = -1 * (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) + (key == GLFW_KEY_W || key == GLFW_KEY_UP); // key W / UP ARROW pressed -> signy = -1 (moving up);  key S / DOWN ARROW pressed -> signy = 1; (moving down)

		switch (key) {	
			case GLFW_KEY_A:
			case GLFW_KEY_LEFT:
			case GLFW_KEY_D:
			case GLFW_KEY_RIGHT:
				off.x += signx * 0.01 * lenx;
				break;

			case GLFW_KEY_S:
			case GLFW_KEY_DOWN:
			case GLFW_KEY_W:
			case GLFW_KEY_UP:
				off.y += signy * 0.01 * leny;
				break;
			
			case GLFW_KEY_I:
				zoom *= 1.1;
				break;

			case GLFW_KEY_O:
				// Prevent zooming out too far
				zoom = std::max(zoom * 0.9, 0.5);
				break;

			// Listen for Esc and close window when key pressed
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;
		}
	}
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset != 0.0f)
		changeCoordRange(window, (bool)(yoffset > 0)); // yoffset > 0 -> zoom in; yoffset < 0 -> zoom out
}


void changeCoordRange(GLFWwindow* window, bool mode) {

	double xMousePos, yMousePos;
	glfwGetCursorPos(window, &xMousePos, &yMousePos);
	yMousePos = currentHeight - yMousePos;
	normalizeCoord(xMousePos, yMousePos);

	// Zoom in/out by 10%
	double zoomFactor = 1.1;

	// Decide whether is a zoom in or a zoom out; zoom in -> sign = -1,  zoom out -> sign = +1
	int sign = (mode == 1) * (-1) + (mode == 0);
	
	// When zooming in, all positions multiply by the zoomFactor
	// When zooming out, all positions divide by the zoomFactor
	// Ex: be a point on the unzoomed(original) screen whose coordinates are (x, y)
	// When zooming in by a factor of 2, the same point on the screen will coincide with the point with the coordinates (x/2, y/2) on the original(unzoomed) screen
	double power = (sign == 1) ? zoomFactor : 1 / zoomFactor;

	// Find the new coordinates of the screen center in the cartesian system
	// Scale the entire image and find which are the new coordinates of the screen center, 
	// then adjust it so that the pixel under the cursor has the same position as before the scaling
	
	off.x = off.x * power + xMousePos * (1 - power);
	off.y = off.y * power + yMousePos * (1 - power);
	zoom *= 1 / power;

	// Prevent zooming out too far
	zoom = std::max(zoom, 0.5);
}


void normalizeCoord(double& x, double& y) {
	double leny = 4;
	double lenx = (1.0 * currentWidth / currentHeight) * leny; // multiply the orizontal length by the aspect ratio to get an image proportional to the screen
	
	// Compute a factor between -0.5 and 0.5 to determine the position of the mouse relative to the center of the screen,
	// then find what the coordinates would be if (0, 0) was the center of the screen and finally add the current coordinates of the screen center
	x = (x / currentWidth - 0.5) * (lenx / zoom) + off.x;
	y = (y / currentHeight - 0.5) * (leny / zoom) + off.y;
}
