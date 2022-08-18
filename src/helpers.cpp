#include "helpers.h"


coord off{ 0.0, 0.0 };
double zoom = 1.0;
int currentWidth, currentHeight;
int maxIterations = 250;


void setWindowCallbacks(GLFWwindow* window) {
	// Set viewport every time the window is resized

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Register the cursor position

	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Register the action of zooming

	glfwSetScrollCallback(window, scroll_callback);

	// Process every key pressed

	glfwSetKeyCallback(window, key_callback);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {}


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

			// Increase iteration count when '+' key(same as '=' key) pressed
		case GLFW_KEY_EQUAL:
			maxIterations = std::min(maxIterations + 10, 2000);
			break;

			// Decrease iteration count when '-' key pressed
		case GLFW_KEY_MINUS:
			maxIterations = std::max(maxIterations - 10, 50);
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
		zoomOnPoint(window, (bool)(yoffset > 0)); // yoffset > 0 -> zoom in; yoffset < 0 -> zoom out
}


void getMouseCoordinates(GLFWwindow* window, double& xMousePos, double& yMousePos) {
	glfwGetCursorPos(window, &xMousePos, &yMousePos);
	yMousePos = currentHeight - yMousePos;
	normalizeCoord(xMousePos, yMousePos);
}


void zoomOnPoint(GLFWwindow* window, bool mode) {

	double xMousePos, yMousePos;
	getMouseCoordinates(window, xMousePos, yMousePos);

	// Zoom in/out by 10%
	double zoomFactor = 1.1;

	// When zooming in, all positions multiply by the zoomFactor
	// When zooming out, all positions divide by the zoomFactor
	// Ex: be a point on the unzoomed(original) screen whose coordinates are (x, y)
	// When zooming in by a factor of 2, the same point on the screen will coincide with the point with the coordinates (x/2, y/2) on the original(unzoomed) screen
	// When zooming out by a factor of 2, the same point on the screen will coincide with the point with the coordinates (2*x, 2*y) on the original(unzoomed) screen
	double power = (mode == 0) ? zoomFactor : 1 / zoomFactor;

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
