#include <imgui/imgui.h>

#include "TestClearColor.hpp"

test::TestClearColor::TestClearColor()
	: color_{ 0.3f, 0.3f, 0.5f, 1.0f }
	, renderer_()
{

}

test::TestClearColor::~TestClearColor()
{

}

void test::TestClearColor::OnUpdate(f32 deltaTime)
{

}

void test::TestClearColor::OnRender()
{
	GLCall(renderer_.ClearColor(color_));
}

void test::TestClearColor::OnGuiRender()
{
	ImGui::ColorEdit4("Clear Color", (f32*)&color_);
}
