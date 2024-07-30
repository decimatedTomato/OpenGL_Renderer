#include "Engine/Renderer.hpp"
#include "Engine/RenderingContext.hpp"
#include "Engine/VertexBuffer.hpp"
#include "Engine/IndexBuffer.hpp"
#include "Engine/VertexArray.hpp"
#include "Engine/Shader.hpp"
#include "Engine/Texture.hpp"

#define COLOR_COUNT_MAX 10

struct Vertex {
	f32 a_pos[3];
	f32 a_col[4];
	f32 a_uv[2];
};

static constexpr Vec3f ColorFromHex(i32 hex)
{
	return {
		((hex & 0xFF0000) >> 16) / 255.f,
		((hex & 0x00FF00) >> 8) / 255.f,
		((hex & 0x0000FF) >> 0) / 255.f,
	};
}

i32 main(void)
{
	RenderingContext context(1600, 900);
	//RenderingContext context(720, 480);
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

	Shader islandShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_island.glsl");
	islandShader.Bind();
	islandShader.SetUniform2i("u_resolution", context.GetWindowResolution());
	islandShader.SetUniform1f("u_zoom_out", 10);
	const int terrain_color_count = 6;
	ASSERT(terrain_color_count <= COLOR_COUNT_MAX);
	islandShader.SetUniform1i("u_terrain_color_count", terrain_color_count);
	/*Vec3f terrain_colors[terrain_color_count] = {
		ColorFromHex(0x0505FF),
		ColorFromHex(0x0720AF),
		ColorFromHex(0xFF00FF),
		ColorFromHex(0x00FFFF),
		ColorFromHex(0xFF0000),
	};*/
	Vec3f terrain_colors[terrain_color_count] = {
		ColorFromHex(0x0720AF),			// Deep ocean
		ColorFromHex(0x0505FF),			// Shallow ocean
		ColorFromHex(0xDBDA81),			// Beach
		ColorFromHex(0x209030),			// Forest
		ColorFromHex(0x3F3F3F),			// Mountain
		ColorFromHex(0xDFDFFF),			// Snowy peak
	};
	islandShader.SetUniform3fv("u_terrain_colors", terrain_color_count, terrain_colors);

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		/* Update uniforms */
		islandShader.SetUniform1f("u_time", (float)context.GetTime());

		/* Draw the bound buffer */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		context.SwapBuffers();
		context.PollEvents();
	}

	return 0;
}