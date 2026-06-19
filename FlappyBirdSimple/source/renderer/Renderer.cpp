#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


DrawData::DrawData(Shader* shader) : m_Shader(shader)
{
	m_HasTexture = false;
}

DrawData::DrawData(Shader* shader, Texture* texture) : m_Shader(shader), m_Texture(texture)
{
	m_HasTexture = true;
}

DrawData::DrawData(){}

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_Drawable = true;
	m_IndicesCount = (uint32_t)indices.size();
}

void DrawData::UpdateShader(Shader* shader)
{
	if(m_Shader) m_Shader->UnBind();
	m_Shader = shader;
}

void DrawData::UpdateTexture(Texture* texture)
{
	if (!texture) 
	{
		m_HasTexture = false;
		return;
	}
	if(m_Texture) m_Texture->UnBind();
	m_Texture = texture;
	m_HasTexture = true;
}


void Renderer::Init(RendererAPI api)
{
	s_CurrentAPI = api;
	if (s_CurrentAPI == OPENGL)
	{
		int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(!result, "FAILES TO LOAD OPENGL!");

		glfwGetFramebufferSize(glfwGetCurrentContext(), &s_FrameBufferWidth, &s_FrameBufferHeight);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

		glfwGetFramebufferSize(glfwGetCurrentContext(), &s_FrameBufferWidth, &s_FrameBufferHeight);
		glViewport(0, 0, s_FrameBufferWidth, s_FrameBufferHeight);
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
			drawData->m_Shader->Bind();

			// Setup matrices
			glm::mat4 projectionMatrix = glm::mat4(1.0f);
			projectionMatrix = glm::ortho(0.0f, (float)s_FrameBufferWidth, 0.0f, (float)s_FrameBufferHeight, -1.0f, 1.0f);
			drawData->m_Shader->SetUniform(Shader::Mat4, "u_ProjMat", projectionMatrix);

			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, drawData->Position);
			modelMatrix = glm::scale(modelMatrix, drawData->Scale);
			drawData->m_Shader->SetUniform(Shader::Mat4, "u_ModelMat", modelMatrix);

			// Isolate texture bindings per entity
			if (drawData->m_HasTexture)
			{
				glActiveTexture(GL_TEXTURE0 + s_TextureSlot); // Lock to binding slot 0
				drawData->m_Texture->Bind();
				drawData->m_Shader->SetUniform(Shader::Int, "u_BaseTexture", s_TextureSlot);
				drawData->m_Shader->SetUniform(Shader::Vec2, "u_Tile", drawData->m_Texture->m_Tile);
				s_TextureSlot++;
			}
			else
			{
				// FIX: If the shape doesn't use a texture, break the bind on slot 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glDrawElements(GL_TRIANGLES, drawData->m_IndicesCount, GL_UNSIGNED_INT, nullptr);
			drawData->m_Shader->UnBind();

			// CRITICAL FIX: Unlock the data asset configuration state for the next frame
			drawData->m_ExistInDrawCall = false;
		}

		// CRITICAL FIX: Empty the draw list queue container completely before starting a new frame
		s_TextureSlot = 0;
		s_DataToDraw.clear();

		glfwSwapBuffers(glfwGetCurrentContext());
	}
}
