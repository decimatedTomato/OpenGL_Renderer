#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

#include "defines.hpp"

#define ASSERT(X) if (!(X)) __debugbreak();
#define GLCall(X) GLClearError();	\
	X;								\
	ASSERT(GLLogCall(#X, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, i32 line);