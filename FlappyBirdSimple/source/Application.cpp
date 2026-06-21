#include "Application.h"
#include "renderer/Renderer.h"
#include "renderer/ImGui.h"
#include "ResourceManager.h"
#include <miniaudio.h>

Application::Application()
{
	m_Window = std::make_shared<Window>(WindowConfig(500, 800, "Flappy Bird Simple"));
	m_AudioEngine = std::make_unique<ma_engine>();
	Renderer::Init(Renderer::OPENGL);
	ResourceManager::Init();
	ImGuiLayer::Init();

	m_Clock = std::make_unique<Time>();

	ma_result result = ma_engine_init(NULL, m_AudioEngine.get());
	if (result != MA_SUCCESS) {
		std::println("Failed to initialize audio engine.");
	}

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
	ResourceManager::LoadResource<Texture>("Red_Bird_Midflap_Texture",	 RESOURCE_PATH"Assets/sprites/redbird-midflap.png");

	m_GameScene = std::make_unique<GameScene>(m_Window, m_AudioEngine.get());
}

Application::~Application()
{
	ImGuiLayer::Shutdown();
	ma_engine_uninit(m_AudioEngine.get());
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