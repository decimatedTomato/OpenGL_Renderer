#include "Renderer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

/// <summary>
/// Compiles the given source code as a shader of the given type
/// </summary>
/// <param name="type">Shader type (as specified in glew.h)</param>
/// <param name="source">Source code of a shader</param>
/// <returns>Shader ID if successful, otherwise 0</returns>
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	/* Error handling */
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(sizeof(char) * length);
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") <<
			" shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath)
{
	std::ifstream t1(vertexShaderFilepath);
	std::string vertexShader((std::istreambuf_iterator<char>(t1)), std::istreambuf_iterator<char>());
	std::ifstream t2(fragmentShaderFilepath);
	std::string fragmentShader((std::istreambuf_iterator<char>(t2)), std::istreambuf_iterator<char>());
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	/* Create a GLFW debug context */
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (GLEW_OK != glewInit())
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "glewInit failed, something is seriously wrong." << std::endl;
	}

	/* Sets the debug messages to run a certain callback function */
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Best to initialize the triangles to be drawn outside of the game loop */
	/*float triangle_positions[] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};*/

	enum Attributes {
		VERTEX_POSITION_ATTRIBUTE = 0,
		VERTEX_COLOR_ATTRIBUTE,
		VERTEX_UV_ATTRIBUTE,
		COUNT_VERTEX_ATTRIBUTES
	};
	struct Vertex {
		float aPosition[3];
		float aVertexColor[4];
		float aUV[2];
	};

	Vertex square_attributes[] = {
		{
			{ -1.0f, -1.0f, 0.0f },		// position
			{ 1.0f, 1.0f, 1.0f, 1.0f },	// color
			{ 0.0f, 0.0f },				// texture coordinate
		},
		{
			{ 1.0f, -1.0f, 0.0f },		// position
			{ 1.0f, 1.0f, 1.0f, 1.0f },	// color
			{ 1.0f, 0.0f },				// texture coordinate
		},
		{
			{ 1.0f, 1.0f, 0.0f },		// position
			{ 1.0f, 1.0f, 1.0f, 1.0f },	// color
			{ 1.0f, 1.0f },				// texture coordinate
		},
		{
			{ -1.0f, 1.0f, 0.0f },		// position
			{ 1.0f, 1.0f, 1.0f, 1.0f },	// color
			{ 0.0f, 1.0f},				// texture coordinate
		},
	};
	unsigned int square_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// TRIANGLE
   /* unsigned int triangle_buffer;
	glGenBuffers(1, &triangle_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), triangle_positions, GL_STATIC_DRAW);

	glVEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	*/

	// SQUARE
	unsigned int vertex_array_object;
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	unsigned int square_buffer;
	glGenBuffers(1, &square_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, square_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_attributes), square_attributes, GL_STATIC_DRAW);

	unsigned int index_buffer_object;
	glGenBuffers(1, &index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), square_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VERTEX_POSITION_ATTRIBUTE);
	glVertexAttribPointer(VERTEX_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, aPosition));

	glEnableVertexAttribArray(VERTEX_COLOR_ATTRIBUTE);
	glVertexAttribPointer(VERTEX_COLOR_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, aVertexColor));

	glEnableVertexAttribArray(VERTEX_UV_ATTRIBUTE);
	glVertexAttribPointer(VERTEX_UV_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, aUV));

	unsigned int defaultShader = CreateShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_basic.glsl");
	glUseProgram(defaultShader);

	int defaultShaderTimeUniformLocation = glGetUniformLocation(defaultShader, "uTime");
	//assert(defaultShaderTimeUniformLocation != -1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Draw the bound buffer */

		/* Update uniforms */
		glUniform1f(defaultShaderTimeUniformLocation, glfwGetTime());

		/* Without an index buffer TRIANGLE */
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		/* With an index buffer SQUARE */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // Since the index buffer is already bound to the element array buffer it does not need to be passed as an argument here

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(defaultShader);

	glfwTerminate();
	return 0;
}