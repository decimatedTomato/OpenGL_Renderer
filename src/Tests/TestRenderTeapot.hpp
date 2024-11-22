#pragma once
#include "glm/glm.hpp"
//#include "glm/gtc/type_ptr.hpp"
//#include "glm/gtc/matrix_transform.hpp"

#include "../Engine/Defines.hpp"
#include "../Engine/Renderer.hpp"
#include "../Engine/Model.hpp"

#include "Test.hpp"

namespace test {

	class TestRenderTeapot : public Test
	{
	public:
		TestRenderTeapot();
		~TestRenderTeapot();

		void OnUpdate(f32 deltaTime) override;
		void OnRender() override;
		void OnGuiRender() override;
	private:
		Renderer renderer_;
		glm::vec3 plane_translation_;
		glm::vec3 plane_rotation_;
		glm::vec3 plane_scale_;
		Shader shader_;
		Model teapot_;
	};
}