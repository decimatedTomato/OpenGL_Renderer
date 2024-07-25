#include "Renderer.hpp"
#include "RenderingContext.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

struct Vertex {
	f32 aPosition[3];
	f32 aVertexColor[4];
	f32 aUV[2];
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

	//Shader defaultShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_basic.glsl");
	srand((u32)time(NULL));
	Shader voronoiShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_voronoi.glsl");
	voronoiShader.Bind();
	voronoiShader.SetUniform1i("uPointCount", 32);
	voronoiShader.SetUniform2i("uResolution", context.GetWindowResolution());
	f32 pointPositions[64] = { 0 };
	for (i32 i = 0; i < 64; i++) pointPositions[i] = static_cast <f32> (rand()) / static_cast <f32> (RAND_MAX);
	voronoiShader.SetUniform2fv("uPointPositions", 32, (Vec2f*)pointPositions);
	f32 pointColors[96] = { 0 };
	for (i32 i = 0; i < 96; i++) pointColors[i] = static_cast <f32> (rand()) / static_cast <f32> (RAND_MAX);
	voronoiShader.SetUniform3fv("uPointColors", 32, (Vec3f*)pointPositions);
	f32 pointVelocities[64] = { 0 };
	for (i32 i = 0; i < 64; i++) pointVelocities[i] = static_cast <f32> (rand()) / static_cast <f32> (RAND_MAX) - 0.5f;
	voronoiShader.SetUniform2fv("uPointVelocities", 32, (Vec2f*)pointVelocities);
	voronoiShader.Unbind();

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		/* Update uniforms */
		//defaultShader.Bind();
		//defaultShader.SetUniform1f("uTime", (float)context.GetTime());
		voronoiShader.Bind();
		voronoiShader.SetUniform1f("uTime", (float)context.GetTime());

		/* Draw the bound buffer */
		va.Bind();
		ib.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // Since the index buffer is already bound to the element array buffer it does not need to be passed as an argument here
		ib.Unbind();
		va.Unbind();
		//defaultShader.Unbind();
		voronoiShader.Unbind();

		context.SwapBuffers();
		context.PollEvents();
	}

	return 0;
}