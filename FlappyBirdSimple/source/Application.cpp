#include "Application.h"
#include "renderer/Renderer.h"
#include "renderer/ImGui.h"

Application::Application()
{
	m_Window = std::make_shared<Window>(WindowConfig(500, 800, "Flappy Bird Simple"));
	Renderer::Init(Renderer::OPENGL);
	ImGuiLayer::Init();

	m_GameScene = std::make_unique<GameScene>(m_Window);
	m_Clock = std::make_unique<Time>();
}

Application::~Application()
{
	ImGuiLayer::Shutdown();
}

void Application::Run() 
{
	while (m_Running) 
	{
		m_Clock->Tick();
		m_Window->PollEvents();

		Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::StartFrame();
		ImGuiLayer::StartFrame();

		m_GameScene->UpdateScene((float)m_Clock->DeltaTime);

		ImGuiLayer::EndFrame();
		Renderer::EndFrame();
	}
}