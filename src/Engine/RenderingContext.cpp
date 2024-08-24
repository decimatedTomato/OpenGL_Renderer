#include "RenderingContext.hpp"

static void APIENTRY glDebugOutput(GLenum source, GLenum type, u32 id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	RenderingContext* context = static_cast<RenderingContext*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		context->ToggleFullscreen();
	}
}

static void OnResize(GLFWwindow* window, i32 w, i32 h)
{
	RenderingContext* context = static_cast<RenderingContext*>(glfwGetWindowUserPointer(window));
	context->SetWindowResolution({ w, h });

	/* Resize viewport */
	context->ResetViewport();

	if (context->onResizeHook_ == nullptr) return;
	context->onResizeHook_(context->onResizeHookParameter_, w, h);
}

static void OnReposition(GLFWwindow* window, int xpos, int ypos)
{
	RenderingContext* context = static_cast<RenderingContext*>(glfwGetWindowUserPointer(window));
	if (context->onRepositionHook_ == nullptr) return;
	context->onRepositionHook_(context->onRepositionHookParameter_, xpos, ypos);
}

RenderingContext::RenderingContext(i32 width, i32 height)
	: initializationError_(0)
	, isFullscreen_(false)
	, monitor_(nullptr)
	, window_(nullptr)
	, monitorData_()
	, windowData_()
	, onResizeHook_(nullptr)
	, onResizeHookParameter_(nullptr)
	, onRepositionHook_(nullptr)
	, onRepositionHookParameter_(nullptr)
{
	/* Initialize the library */
	if (!glfwInit())
	{
		initializationError_ = -1;
		return;
	}
	/* Create a GLFW debug context */
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Set monitor reference */
	monitor_ = glfwGetPrimaryMonitor();
	if (!monitor_)
	{
		initializationError_ = -1;
		return;
	}
	InitMonitorData();

	/* Create a windowed mode window and its OpenGL context */
	window_ = glfwCreateWindow(width, height, "OpenGL Renderer", NULL, NULL);
	if (!window_)
	{
		glfwTerminate();
		initializationError_ = -1;
		return;
	}
	InitWindowData();

	/* Make the window's context current */
	glfwMakeContextCurrent(window_);

	/* Make viewport support resizing */
	//TODO

	/* Allow two way access */
	glfwSetWindowUserPointer(window_, this);

	/* Set GLFW input callbacks */
	glfwSetKeyCallback(window_, &KeyCallback);
	glfwSetWindowSizeCallback(window_, &OnResize);
	glfwSetWindowPosCallback(window_, &OnReposition);


	if (GLEW_OK != glewInit())
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		initializationError_ = -1;
	}

	/* Sets the debug messages to run a certain callback function */
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

RenderingContext::~RenderingContext()
{
	glfwTerminate();
}

bool RenderingContext::InitializationFailed() const
{
	return initializationError_;
}

bool RenderingContext::ShouldWindowClose() const
{
	if (initializationError_) return true;
	return glfwWindowShouldClose(window_);
}

void RenderingContext::SwapBuffers() const
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window_);
}

void RenderingContext::PollEvents() const
{
	/* Poll for and process events */
	glfwPollEvents();
}

double RenderingContext::GetTime() const
{
	return glfwGetTime();
}

Vec2i RenderingContext::GetWindowResolution() const
{
	if (isFullscreen_)
	{
		return monitorData_.monitorResolution;
	}
	else
	{
		return windowData_.windowedModeResolution;
	}
}

void RenderingContext::SetWindowResolution(Vec2i res)
{
	if (!isFullscreen_)
	{
		windowData_.windowedModeResolution = res;
	}
}

void RenderingContext::ResetViewport() const
{
	if (!isFullscreen_)
	{
		glViewport(0, 0, windowData_.windowedModeResolution.x, windowData_.windowedModeResolution.y);
		//glViewport(windowData_.windowedModeCoordinates.x, windowData_.windowedModeCoordinates.y, windowData_.windowedModeResolution.x, windowData_.windowedModeResolution.y);
	}
	else
	{
		glViewport(0, 0, monitorData_.monitorResolution.x, monitorData_.monitorResolution.y);
	}
}

void RenderingContext::ToggleFullscreen()
{
	isFullscreen_ ^= 1;
	if (!isFullscreen_)
	{
		glfwSetWindowMonitor(window_, NULL, windowData_.windowedModeCoordinates.x, windowData_.windowedModeCoordinates.y, windowData_.windowedModeResolution.x, windowData_.windowedModeResolution.y, GLFW_DONT_CARE);
	}
	else
	{
		glfwSetWindowMonitor(window_, monitor_, 0, 0, monitorData_.monitorResolution.x, monitorData_.monitorResolution.y, GLFW_DONT_CARE);
	}
}

void RenderingContext::InitMonitorData()
{
	const GLFWvidmode* mode = glfwGetVideoMode(monitor_);
	monitorData_.monitorResolution = { mode->width, mode->height };
}

void RenderingContext::InitWindowData()
{
	glfwGetWindowPos(window_, &windowData_.windowedModeCoordinates.x, &windowData_.windowedModeCoordinates.y);
	glfwGetWindowSize(window_, &windowData_.windowedModeResolution.x, &windowData_.windowedModeResolution.y);
}


void RenderingContext::SetOnResize(ResizeHook hook, void* data)
{
	ASSERT(hook != nullptr);
	onResizeHook_ = hook;
	onResizeHookParameter_ = data;
}