#include "Application.h"
#include "renderer/Renderer.h"
#include "renderer/ImGui.h"

Application::Application()
{
	m_Window = std::make_shared<Window>(WindowConfig(1280, 720, "Flappy Bird Simple"));

	Renderer::Init(Renderer::OPENGL);
	ImGuiLayer::Init();
}

Application::~Application()
{
	ImGuiLayer::Shutdown();
}

void Application::Run() 
{
	Shader shader(RESOURCE_PATH"vertexshader.glsl", RESOURCE_PATH"fragmentshader.glsl");
	DrawData drawData1(shader);

	std::vector<float> vertices =
	{
		 100.0f, -100.0f, 0.0f,
		-100.0f, -100.0f, 0.0f,
		-100.0f,  100.0f, 0.0f,
		 100.0f,  100.0f, 0.0f,
	};
	std::vector<uint32_t> indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	drawData1.GenerateDrawData(vertices, indices);

	while (m_Running) 
	{
		m_Window->PollEvents();


		Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::StartFrame();
		ImGuiLayer::StartFrame();

		Renderer::Draw(drawData1);
		ImGui::Begin("Test Window");
		ImGui::Text("Temp Text");
		ImGui::End();


		ImGuiLayer::EndFrame();
		Renderer::EndFrame();

	}
}