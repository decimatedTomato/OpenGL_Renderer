#include "Engine/Renderer.hpp"
#include "Engine/RenderingContext.hpp"
#include "Engine/VertexBuffer.hpp"
#include "Engine/IndexBuffer.hpp"
#include "Engine/VertexArray.hpp"
#include "Engine/Shader.hpp"
#include "ApplicationVoronoi.h"

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

	const u32 square_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	IndexBuffer ib(square_indices, 6);

	srand((u32)time(NULL));
	Shader voronoiShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_voronoi.glsl");
	voronoiShader.Bind();
	voronoiShader.SetUniform1i("u_p_count", 32);
	voronoiShader.SetUniform2i("u_resolution", context.GetWindowResolution());
	f32 pointPositions[64] = { 0 };
	for (i32 i = 0; i < 64; i++) pointPositions[i] = RandomFloat();
	voronoiShader.SetUniform2fv("u_p_pos", 32, (Vec2f*)pointPositions);
	f32 pointColors[96] = { 0 };
	for (i32 i = 0; i < 96; i++) pointColors[i] = RandomFloat() + 0.1;
	for (int i = 0; i < 96; i++) std::cout << pointColors[i] << ", ";
	voronoiShader.SetUniform3fv("u_p_col", 32, (Vec3f*)pointPositions);
	f32 pointVelocities[64] = { 0 };
	for (i32 i = 0; i < 64; i++) pointVelocities[i] = RandomFloat() - 0.5f;
	voronoiShader.SetUniform2fv("u_p_vel", 32, (Vec2f*)pointVelocities);
	voronoiShader.Unbind();

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		/* Update uniforms */
		voronoiShader.Bind();
		voronoiShader.SetUniform1f("u_time", (float)context.GetTime());

		/* Draw the bound buffer */
		va.Bind();
		ib.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		ib.Unbind();
		va.Unbind();
		voronoiShader.Unbind();

		context.SwapBuffers();
		context.PollEvents();
	}

	return 0;
}

static float RandomFloat()
{
	return static_cast <f32> (rand()) / static_cast <f32> (RAND_MAX);
}
