#include "Window.h"
#include <GLFW/glfw3.h>
#include <renderer/Renderer.h>

Window::Window(WindowConfig config)
{
	int versionMajor = 4, versionMinor = 6;

	int result = glfwInit();
	ASSERT(!result, "FAILED TO INITIALIZE GLFW");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), nullptr, nullptr);

	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
			Renderer::ResizeFrameBuffer(width, height);
		});

	glfwMakeContextCurrent(m_Window);
	m_Config = std::move(config);
}
Window::~Window() 
{
	glfwDestroyWindow(m_Window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}
