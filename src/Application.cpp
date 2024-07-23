#include "Renderer.hpp"
#include "RenderingContext.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

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
	RenderingContext context;
	if (context.InitializationFailed())
	{
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;


	struct Vertex {
		float aPosition[3];
		float aVertexColor[4];
		float aUV[2];
	};

	const Vertex square_attributes[] = {
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

	VertexArray va;
	VertexBuffer vb(square_attributes, sizeof(square_attributes));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	va.Bind();

	const unsigned int square_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	IndexBuffer ib(square_indices, 6);
	ib.Bind();

	unsigned int defaultShader = CreateShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_basic.glsl");
	glUseProgram(defaultShader);

	int defaultShaderTimeUniformLocation = glGetUniformLocation(defaultShader, "uTime");
	assert(defaultShaderTimeUniformLocation != -1);

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Draw the bound buffer */

		/* Update uniforms */
		glUniform1f(defaultShaderTimeUniformLocation, (float)glfwGetTime());

		/* Without an index buffer TRIANGLE */
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		/* With an index buffer SQUARE */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // Since the index buffer is already bound to the element array buffer it does not need to be passed as an argument here

		context.SwapBuffers();
		context.PollEvents();
	}
	glDeleteProgram(defaultShader);

	return 0;
}