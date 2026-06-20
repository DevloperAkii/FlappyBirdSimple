#include "Application.h"
#include "renderer/Renderer.h"
#include "renderer/ImGui.h"
#include "ResourceManager.h"

Application::Application()
{
	m_Window = std::make_shared<Window>(WindowConfig(500, 800, "Flappy Bird Simple"));
	Renderer::Init(Renderer::OPENGL);
	ResourceManager::Init();
	ImGuiLayer::Init();

	m_Clock = std::make_unique<Time>();

	ResourceManager::LoadResource<Shader>("Color_Shader",
	RESOURCE_PATH"Assets/shaders/ColorVertexShader.glsl", RESOURCE_PATH"Assets/shaders/ColorFragmentShader.glsl");

	ResourceManager::LoadResource<Shader>("Texture_Shader",
	RESOURCE_PATH"Assets/shaders/TextureVertexShader.glsl", RESOURCE_PATH"Assets/shaders/TextureFragmentShader.glsl");

	ResourceManager::LoadResource<Texture>("Background_Texture", RESOURCE_PATH"Assets/sprites/background-day.png");

	ResourceManager::LoadResource<Texture>("Base_Texture", RESOURCE_PATH"Assets/sprites/base.png");
	ResourceManager::LoadResource<Texture>("Pipe_Texture", RESOURCE_PATH"Assets/sprites/pipe-green.png");

	ResourceManager::LoadResource<Texture>("Yellow_Bird_Upflap_Texture",	 RESOURCE_PATH"Assets/sprites/yellowbird-upflap.png");
	ResourceManager::LoadResource<Texture>("Yellow_Bird_Middleflap_Texture", RESOURCE_PATH"Assets/sprites/yellowbird-midflap.png");
	ResourceManager::LoadResource<Texture>("Yellow_Bird_Downflap_Texture",	 RESOURCE_PATH"Assets/sprites/yellowbird-downflap.png");

	m_GameScene = std::make_unique<GameScene>(m_Window);
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

		Renderer::EndFrame();
		ImGuiLayer::EndFrame();
		Renderer::SwapFrameBuffer();
	}
}