#pragma once
#include "Renderer.hpp"

class RenderingContext
{
private:
	int initializationError_;
	GLFWwindow* window_;
public:
	RenderingContext();
	~RenderingContext();

	bool InitializationFailed() const;
	bool ShouldWindowClose() const;
	void SwapBuffers() const;
	void PollEvents() const;
	double GetTime() const;
};