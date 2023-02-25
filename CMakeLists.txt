cmake_minimum_required(VERSION 3.0)
project(mandelbrot-opengl)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES src/main.cpp src/helpers.cpp src/shader.cpp thirdparty/glad/src/glad.c)

include_directories(thirdparty/glad/include include)
include_directories(thirdparty/glfw-3.3.8/include)
add_executable(mandelbrot-opengl ${SOURCE_FILES})
target_link_libraries(mandelbrot-opengl ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw-3.3.8/build/src/Debug/glfw3.lib)
