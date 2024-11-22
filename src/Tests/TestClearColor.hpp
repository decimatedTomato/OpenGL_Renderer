#pragma once

#include "../Engine/Defines.hpp"
#include "../Engine/Renderer.hpp"

#include "Test.hpp"

namespace test {

	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(f32 deltaTime) override;
		void OnRender() override;
		void OnGuiRender() override;
	private:
		Vec4f color_;
		Renderer renderer_;
	};
}