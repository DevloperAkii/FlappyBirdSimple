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
	void Restart();
private:
	void CheckRestart();
private:
	bool m_Running = true;
	bool m_Restart = false;

	std::shared_ptr<Window> m_Window;
	std::unique_ptr<GameScene> m_GameScene;
	std::unique_ptr<Time> m_Clock;
	std::unique_ptr<ma_engine> m_AudioEngine;

	float DeltaTime = 0.0f;
};