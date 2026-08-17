#include "Window.h"
#include "renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

Window::Window(WindowConfig config)
{
	int versionMajor = 4, versionMinor = 6;

	int result = glfwInit();
	ASSERT(!result, "FAILED TO INITIALIZE GLFW");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), nullptr, nullptr);

	glfwSetWindowUserPointer(m_Window, this);

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
			Window* user = (Window*)glfwGetWindowUserPointer(window);
			user->m_Config.Width = width;
			user->m_Config.Height = height;
			Renderer::ResizeFrameBuffer(width, height);
		});

	glfwMakeContextCurrent(m_Window);

	HWND hwnd = glfwGetWin32Window(m_Window);

	if (hwnd) {
		// 3. Load the icon using the string identifier "MAINICON" from your resource.rc
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), "MAINICON");

		if (hIcon) {
			// 4. Send messages to the window to update the title bar and taskbar icons
			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		}
	}

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