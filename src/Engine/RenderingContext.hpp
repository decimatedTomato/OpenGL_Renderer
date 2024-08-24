#pragma once
#include "Defines.hpp"

typedef void(*ResizeHook)(void*, int, int);
typedef void(*RepositionHook)(void*, int, int);

struct MonitorData
{
	Vec2i monitorResolution;
};
struct WindowData
{
	Vec2i windowedModeCoordinates;
	Vec2i windowedModeResolution;
};

class RenderingContext
{
private:
	i32 initializationError_;
	b32 isFullscreen_;
	GLFWmonitor* monitor_;
	GLFWwindow* window_;
	MonitorData monitorData_;
	WindowData windowData_;

public:
	ResizeHook onResizeHook_;
	void* onResizeHookParameter_;
	RepositionHook onRepositionHook_;
	void* onRepositionHookParameter_;

	RenderingContext(i32 width, i32 height);
	~RenderingContext();

	bool InitializationFailed() const;
	bool ShouldWindowClose() const;
	void SwapBuffers() const;
	void PollEvents() const;

	double GetTime() const;
	Vec2i GetWindowResolution() const;
	void SetWindowResolution(Vec2i res);
	void ResetViewport() const;

	void ToggleFullscreen();

	void SetOnResize(ResizeHook hook, void* data);
private:
	void InitMonitorData();
	void InitWindowData();
};