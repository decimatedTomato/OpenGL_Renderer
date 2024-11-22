#include <imgui/imgui.h>

#include "TestRenderTeapot.hpp"

test::TestRenderTeapot::TestRenderTeapot()
	: renderer_()
	, plane_translation_{ 0.0f, 0.0f, 0.0f }
	, plane_rotation_{ 0.0f, 0.0f, 0.0f }
	, plane_scale_{ 1.0f, 1.0f, 1.0f }
	, shader_{ "res/shaders/vertex_basic.glsl", "res/shaders/fragment_rainbow.glsl" }
	//, teapot_("res/models/teapot.obj")
	, teapot_("res/models/plane.obj")
{
}

test::TestRenderTeapot::~TestRenderTeapot()
{
}

void test::TestRenderTeapot::OnUpdate(f32 deltaTime)
{

}

void test::TestRenderTeapot::OnRender()
{
	GLCall(renderer_.Clear());
	GLCall(glDrawArrays(GL_TRIANGLES, 0, teapot_.getVertexCount()));
	//renderer_.Draw(teapot_.getVertexArray(), teapot_.getIndexBuffer(), shader_);
}

void test::TestRenderTeapot::OnGuiRender()
{
	ImGui::Begin("Teapot transform");
	{
		ImGui::BeginGroup();
		ImGui::SliderFloat3("Translation", (f32*)&plane_translation_, -2.0f, 2.0f);
		ImGui::SliderFloat3("Rotation", (f32*)&plane_rotation_, -1.0f, 1.0f);
		//ImGui::SliderFloat3("Rotation", (f32*)&plane_rotation_, -.01f, .01f);
		ImGui::SliderFloat3("Scale", (f32*)&plane_scale_, -5.0f, 5.0f);
		ImGui::EndGroup();
	}
	ImGui::End();
}
