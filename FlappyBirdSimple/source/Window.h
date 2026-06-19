#pragma once
#include "PCH.h"
#include <string>

struct WindowConfig 
{
	int Width = 1280;
	int Height = 720;
	std::string Title = "Flappy Bird Simple";
};

struct GLFWwindow;

class Window 
{
public:
	Window(WindowConfig config);
	~Window();
	void PollEvents();
private:
	friend class Renderer;
	friend class GameScene;

	GLFWwindow* m_Window = nullptr;

	WindowConfig m_Config;
};