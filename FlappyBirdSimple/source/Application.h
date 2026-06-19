#pragma once
#include "Window.h"
#include "GameScene.h"
#include "Timer.h"

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
	std::unique_ptr<GameScene> m_GameScene;
	std::unique_ptr<Time> m_Clock;

	float DeltaTime = 0.0f;
};