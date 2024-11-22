#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

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

	//Texture texture("res/textures/African-savanna-elephant.png");
	//texture.Bind(0);

	glm::vec3 plane_translation(0.0f, 0.0f, 0.0f);
	glm::vec3 plane_rotation(0.0f, 0.0f, 0.0f);
	glm::vec3 plane_scale(1.0f, 1.0f, 1.0f);

	const glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	//const glm::mat4 proj = glm::perspective(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glm::mat4 view;
	if (plane_rotation == glm::vec3(0.0f, 0.0f, 0.0f))
	{
		view = glm::scale(glm::translate(glm::mat4(1.0f), plane_translation), plane_scale);
	}
	else
	{
		view = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), plane_translation), glm::pi<f32>(), glm::normalize(plane_rotation)), plane_scale);
	}
	const glm::mat4 model(1.0f);
	glm::mat4 mvp = proj * view * model;
	Shader distortionShader("res/shaders/vertex_standard.glsl", "res/shaders/fragment_standard.glsl");
	distortionShader.Bind();
	distortionShader.SetUniformMat4x4f("u_model_view_projection", glm::value_ptr(mvp));

	/* Loop until the user closes the window */
	while (!context.ShouldWindowClose())
	{
		renderer.Clear();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* Draw the bound buffer */
		if (plane_rotation == glm::vec3(0.0f, 0.0f, 0.0f))
		{
			view = glm::scale(glm::translate(glm::mat4(1.0f), plane_translation), plane_scale);
		}
		else
		{
			view = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), plane_translation), glm::pi<f32>(), glm::normalize(plane_rotation)), plane_scale);
		}
		mvp = proj * view * model;
		distortionShader.SetUniformMat4x4f("u_model_view_projection", glm::value_ptr(mvp));

		renderer.Draw(va, ib, distortionShader);

		ImGui::Begin("Config");
		{
			ImGui::BeginGroup();
			ImGui::SliderFloat3("Translation", (f32*)&plane_translation, -2.0f, 2.0f);
			//ImGui::SliderFloat3("Rotation", (f32*)&plane_rotation, -1.0f, 1.0f);
			ImGui::SliderFloat3("Rotation", (f32*)&plane_rotation, -.01f, .01f);
			ImGui::SliderFloat3("Scale", (f32*)&plane_scale, -5.0f, 5.0f);
			ImGui::EndGroup();
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		context.SwapBuffers();
		context.PollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}