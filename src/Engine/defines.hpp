#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>
#include <cstdint>
#include <cfloat>
#include <cstddef>

using byte = char;

using c16 = char16_t;

using b32 = std::uint32_t;

using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using size = std::uint32_t;
using std::ptrdiff_t;

struct Vec2f
{
	f32 x, y;
};
struct Vec3f
{
	f32 x, y, z;
};
struct Vec2i
{
	i32 x, y;
};

void GLClearError();
bool GLLogCall(const char* function, const char* file, i32 line);

#define ASSERT(X) if (!(X)) __debugbreak();
#define GLCall(X) GLClearError();	\
	X;								\
	ASSERT(GLLogCall(#X, __FILE__, __LINE__))