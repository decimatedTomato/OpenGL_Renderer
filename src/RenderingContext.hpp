#pragma once
#include "Renderer.hpp"

class RenderingContext
{
private:
	i32 initializationError_;
	GLFWwindow* window_;
public:
	RenderingContext(int width, int height);
	~RenderingContext();

	bool InitializationFailed() const;
	bool ShouldWindowClose() const;
	void SwapBuffers() const;
	void PollEvents() const;

	double GetTime() const;
	Vec2i GetWindowResolution() const;
};