#pragma once

#include "Window.h"
#include <memory>

class Application
{
public:
	Application();
	~Application();
	void Run();
private:
	bool m_Running = true;
	std::shared_ptr<Window> m_Window;
};