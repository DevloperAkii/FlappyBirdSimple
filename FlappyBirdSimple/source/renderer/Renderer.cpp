#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


DrawData::DrawData(Shader& shader) : m_Shader(shader)
{

}

DrawData::~DrawData()
{

}

void DrawData::GenerateDrawData(std::vector<float>& vertices, std::vector<uint32_t>& indices)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_Drawable = true;
	m_IndicesCount = (uint32_t)indices.size();
}


void Renderer::Init(RendererAPI api)
{
	s_CurrentAPI = api;
	if (s_CurrentAPI == OPENGL)
	{
		int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(!result, "FAILES TO LOAD OPENGL!");

		glfwGetFramebufferSize(glfwGetCurrentContext(), &s_FrameBufferWidth, &s_FrameBufferHeight);
	}
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	s_ClearColorRed = r; s_ClearColorGreen = g, s_ClearColorBlue = b, s_ClearColorAlpha = a;
}

void Renderer::StartFrame()
{
	if (s_CurrentAPI == OPENGL) 
	{
		glClearColor(s_ClearColorRed, s_ClearColorGreen, s_ClearColorBlue, s_ClearColorAlpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void Renderer::Draw(DrawData& drawData)
{
	if (s_CurrentAPI == OPENGL) 
	{
		if (drawData.m_Drawable && !drawData.m_ExistInDrawCall) 
		{
			s_DataToDraw.push_back(&drawData);
			drawData.m_ExistInDrawCall = true;
		}
	}
}

void Renderer::EndFrame()
{
	if (s_CurrentAPI == OPENGL)
	{
		for (auto& drawData : s_DataToDraw) 
		{
			glBindVertexArray(drawData->m_VAO);

			drawData->m_Shader.Bind();
			s_ProjectionMatrix = glm::ortho(0.0f, (float)s_FrameBufferWidth, 0.0f, (float)s_FrameBufferHeight, -1.0f, 1.0f);
			drawData->m_Shader.SetUniform(Shader::Mat4, "projMat", s_ProjectionMatrix);

			glDrawElements(GL_TRIANGLES, drawData->m_IndicesCount, GL_UNSIGNED_INT, nullptr);
		}
		glfwSwapBuffers(glfwGetCurrentContext());
	}
}