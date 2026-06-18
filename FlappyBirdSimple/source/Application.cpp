#include "Application.h"
#include "renderer/Renderer.h"
#include "renderer/ImGui.h"
#include <glm/gtc/matrix_transform.hpp>

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
	Texture texture(RESOURCE_PATH"yellowbird-midflap.png");
	DrawData drawData1(&shader, &texture);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(1280.0f / 2.0f, 720.0f / 2.0f, 0.0f));
	shader.SetUniform(Shader::Mat4, "u_ModelMat", modelMat);

	std::vector<float> vertices =
	{
		 // Position				// UV Coords 
		 50.0f, -50.0f, 0.0f,		1.0f, 0.0f,
		-50.0f, -50.0f, 0.0f,		0.0f, 0.0f,
		-50.0f,  50.0f, 0.0f,		0.0f, 1.0f,
		 50.0f,  50.0f, 0.0f,		1.0f, 1.0f,
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

		ImGuiLayer::EndFrame();
		Renderer::EndFrame();

	}
}