#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Engine/RenderingContext.hpp"

#include "tests/TestClearColor.hpp"
#include "Tests/TestRenderTeapot.hpp"


i32 main(void)
{
	RenderingContext context(720, 480);
	if (context.InitializationFailed())
	{
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	Renderer renderer;

	test::Test* test_scenes[] = {
		new test::TestRenderTeapot(),
		new test::TestClearColor(),
	};
	u32 scene_count = sizeof(test_scenes) / sizeof(test_scenes[0]);
	i32 current_scene_index = 0;

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(context.GetWindow(), true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	while (!context.ShouldWindowClose())
	{
		renderer.Clear();
		const auto& test = test_scenes[current_scene_index];
		test->OnUpdate(0.0f);
		test->OnRender();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Scene Selector");
		if (ImGui::SmallButton("<"))
		{
			current_scene_index = (current_scene_index - 1) % scene_count;
		}
		ImGui::SameLine();
		ImGui::Text("Scene %i", current_scene_index);
		ImGui::SameLine();
		if (ImGui::SmallButton(">"))
		{
			current_scene_index = (current_scene_index + 1) % scene_count;
		}
		ImGui::End();

		test->OnGuiRender();

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