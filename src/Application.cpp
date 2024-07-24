#include "Renderer.hpp"
#include "RenderingContext.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Application.h"

struct Vertex {
	float aPosition[3];
	float aVertexColor[4];
	float aUV[2];
};

int main(void)
{
	RenderingContext context;
	if (context.InitializationFailed())
	{
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;


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

	const unsigned int square_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	IndexBuffer ib(square_indices, 6);

	Shader defaultShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_basic.glsl");

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		/* Update uniforms */
		defaultShader.Bind();
		defaultShader.SetUniform1f("uTime", (float)context.GetTime());

		/* Draw the bound buffer */
		va.Bind();
		ib.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // Since the index buffer is already bound to the element array buffer it does not need to be passed as an argument here
		ib.Unbind();
		va.Unbind();
		defaultShader.Unbind();

		context.SwapBuffers();
		context.PollEvents();
	}

	return 0;
}