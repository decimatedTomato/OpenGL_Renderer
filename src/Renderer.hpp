#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

#define ASSERT(X) if  (!(X)) __debugbreak();
#define GLCall(X) GLClearError();	\
	X;								\
	ASSERT(GLLogCall(#X, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);