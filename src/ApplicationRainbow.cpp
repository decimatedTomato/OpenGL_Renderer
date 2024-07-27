#include "Engine/Renderer.hpp"
#include "Engine/RenderingContext.hpp"
#include "Engine/VertexBuffer.hpp"
#include "Engine/IndexBuffer.hpp"
#include "Engine/VertexArray.hpp"
#include "Engine/Shader.hpp"

struct Vertex {
	f32 a_pos[3];
	f32 a_col[4];
	f32 a_uv[2];
};

i32 main(void)
{
	RenderingContext context(720, 480);
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
	layout.Push<f32>(3);
	layout.Push<f32>(4);
	layout.Push<f32>(2);
	va.AddBuffer(vb, layout);
	va.Bind();

	const u32 square_indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	IndexBuffer ib(square_indices, 6);
	ib.Bind();

	Shader rainbowShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_rainbow.glsl");
	rainbowShader.Bind();

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		/* Update uniforms */
		rainbowShader.SetUniform1f("u_time", (float)context.GetTime());

		/* Draw the bound buffer */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		context.SwapBuffers();
		context.PollEvents();
	}

	return 0;
}