#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>

struct coord{
	double x, y;
};

extern coord off;
extern double zoom;
extern int currentWidth, currentHeight;
extern int maxIterations;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void zoomOnPoint(GLFWwindow* window, bool mode); // Function used when zooming in/out on a specific point that changes the coordinates of the screen center accordingly
void normalizeCoord(double& x, double& y);  // Function that takes window coordinates and transforms them into real coordinates
void setWindowCallbacks(GLFWwindow* window); // Set all the callbacks for the window
void getMouseCoordinates(GLFWwindow* window, double& xMousePos, double& yMousePos); // Transform the window coordinates of the mouse to real coordinates