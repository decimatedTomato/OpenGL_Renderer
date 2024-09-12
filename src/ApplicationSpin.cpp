#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"

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

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(context.GetWindow(), true);

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

	//float aspect = 16.0f / 9;
	//const glm::mat4 modelviewprojection = glm::mat4(1.0f);
	//const glm::mat4 projection = glm::mat4(1.0f);
	//const glm::mat4 stretched_projection = glm::ortho(1.0f, -1.0f, 1.0f / aspect, -1.0f / aspect, 1.0f, -1.0f);
	//const glm::mat4 proj = glm::ortho(0.0f, (f32)context.GetWindowResolution().x, 0.0f, (f32)context.GetWindowResolution().y, -1.0f, 1.0f);
	const glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	const glm::mat4 model(1.0f);
	const glm::mat4 mvp = proj * view * model;
	Shader distortionShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_standard.glsl");
	distortionShader.Bind();
	distortionShader.SetUniformMat4x4f("u_model_view_projection", glm::value_ptr(mvp));

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		renderer.Clear();

		/* Draw the bound buffer */
		const glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
		const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(glm::sin(context.GetTime()), glm::cos(context.GetTime()), 0.0f));
		const glm::mat4 model(1.0f);
		const glm::mat4 mvp = proj * view * model;
		distortionShader.SetUniformMat4x4f("u_model_view_projection", glm::value_ptr(mvp));

		renderer.Draw(va, ib, distortionShader);

		context.SwapBuffers();
		context.PollEvents();
	}

	return 0;
}