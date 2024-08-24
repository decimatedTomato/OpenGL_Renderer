#include "Engine/Renderer.hpp"
#include "Engine/RenderingContext.hpp"
#include "Engine/VertexBuffer.hpp"
#include "Engine/IndexBuffer.hpp"
#include "Engine/VertexArray.hpp"
#include "Engine/Shader.hpp"
#include "Engine/Texture.hpp"

struct Vertex {
	f32 a_pos[3];
	f32 a_col[4];
	f32 a_uv[2];
};

static void OnResize(void* data, int newWidth, int newHeight)
{
	static_cast<Shader*>(data)->SetUniform2i("u_resolution", { newWidth, newHeight });
}

i32 main(void)
{
	RenderingContext context(720, 480);
	if (context.InitializationFailed())
	{
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	Renderer renderer;

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

	Texture texture("res/textures/African-savanna-elephant.png");
	texture.Bind(0);

	Shader distortionShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_distortion.glsl");
	distortionShader.Bind();
	distortionShader.SetUniform1f("u_time", 0);
	distortionShader.SetUniform2i("u_resolution", context.GetWindowResolution());
	distortionShader.SetUniform1i("u_texture", 0);
	distortionShader.SetUniform2f("u_center", { 0.5f, 0.5f });
	context.SetOnResize(OnResize, &distortionShader);

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		renderer.Clear();

		/* Draw the bound buffer */
		distortionShader.SetUniform1f("u_time", context.GetTime());
		renderer.Draw(va, ib, distortionShader);

		context.SwapBuffers();
		context.PollEvents();
	}

	return 0;
}