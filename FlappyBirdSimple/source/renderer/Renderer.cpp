#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

void DrawData::GenerateDrawData(std::vector<float>& vertices, std::vector<uint32_t>& indices)
{
	GenerateDrawData(vertices);

	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	m_IndicesCount = indices.size();
}

void DrawData::GenerateDrawData(std::vector<float>& vertices)
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

	glBindVertexArray(0);

	m_Drawable = true;
	m_VerticesCount = vertices.size() / 5;
}

void DrawData::UpdateShader(Shader& shader)
{
	if (m_Shader) 
	{
		m_Shader->UnBind();
	}
	m_Shader = &shader;
}

void DrawData::UpdateTexture(Texture& texture)
{
	if (!texture.m_TextureHandel) 
	{
		m_HasTexture = false;
		return;
	}
	if (m_Texture) 
	{
		m_Texture->UnBind();
	};
	m_Texture = &texture;
	m_HasTexture = true;
}

void DrawData::UpdateUniform(Shader::UniformType uniformType, std::string name, std::variant<int, float, glm::vec2, glm::vec3, glm::mat4> value)
{
	m_ShaderUniforms.push_back({ uniformType, name, value});
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

		if (!s_LineData.Generated) 
		{
			glGenVertexArrays(1, &s_LineData.m_DrawData.m_VAO);
			glGenBuffers(1, &s_LineData.m_DrawData.m_VBO);
			s_LineData.m_DrawData.m_Topology = DrawData::Line;
		}
	}
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	s_ClearColorRed = r; s_ClearColorGreen = g, s_ClearColorBlue = b, s_ClearColorAlpha = a;
}

void Renderer::ResizeFrameBuffer(int width, int height)
{
	if (s_CurrentAPI == OPENGL) 
	{
		s_FrameBufferWidth = width;
		s_FrameBufferHeight = height;

		glViewport(0, 0, width, height);
	}
}

void Renderer::GetFrameBuffer(int* width, int* height)
{
	*width = s_FrameBufferWidth;
	*height = s_FrameBufferHeight;
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
		if (drawData.m_Drawable && drawData.m_Shader) 
		{
			if (s_DataToDraw.contains(drawData.m_Shader->m_ShaderHandel)) 
			{
				auto& data = s_DataToDraw[drawData.m_Shader->m_ShaderHandel];
				data.push_back(&drawData);
			}
			else 
			{
				std::vector<DrawData*> drawDatas = {&drawData};
				s_DataToDraw.insert({ drawData.m_Shader->m_ShaderHandel, std::move(drawDatas)});
			}
		}
	}
}

void Renderer::DrawLine(glm::vec3 startPos, glm::vec3 endPos, Shader shader, glm::vec3 color)
{
	if (s_CurrentAPI == OPENGL)
	{
		// 1. Array contains exactly 6 floats (3 elements per point)
		std::vector<float> lineVertices =
		{
			startPos.x, startPos.y, startPos.z,
			endPos.x,   endPos.y,   endPos.z
		};

		glBindVertexArray(s_LineData.m_DrawData.m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, s_LineData.m_DrawData.m_VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lineVertices.size(), lineVertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);

		glEnableVertexAttribArray(0);

		//Temp maybe removed soon
		glDisableVertexAttribArray(1);
		//
		glBindVertexArray(0);

		if (!s_LineData.m_DrawData.m_Shader) 
		{
			s_LineData.m_DrawData.UpdateShader(shader);
		}
		s_LineData.m_DrawData.m_VerticesCount = lineVertices.size() / 3;

		s_LineData.m_DrawData.m_Drawable = true;

		// Set Up Orthographic Projection Matrix
		glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)s_FrameBufferWidth, 0.0f, (float)s_FrameBufferHeight, -1.0f, 1.0f);
		s_LineData.m_DrawData.UpdateUniform(Shader::Mat4, "u_ProjMat", projectionMatrix);

		// FIX: Pass a clean Identity matrix so positions are evaluated raw via absolute pixels
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		s_LineData.m_DrawData.UpdateUniform(Shader::Mat4, "u_ModelMat", modelMatrix);

		// Pass primitive tracking uniforms
		s_LineData.m_DrawData.UpdateUniform(Shader::Vec3, "u_Color", color);

		Draw(s_LineData.m_DrawData);
	}
}


void Renderer::EndFrame()
{
	if (s_CurrentAPI == OPENGL)
	{
		for (const auto& [id, data] : s_DataToDraw)
		{
			for (const auto& drawData : data) {

				glBindVertexArray(drawData->m_VAO);
				drawData->m_Shader->Bind();

				if (drawData->m_HasTexture)
				{
					glActiveTexture(GL_TEXTURE0 + s_TextureSlot);
					drawData->m_Texture->Bind();
					drawData->m_Shader->SetUniform(Shader::Int, "u_BaseTexture", s_TextureSlot);
					s_TextureSlot++;
				}
				else
				{
					glActiveTexture(GL_TEXTURE0);
					if (drawData->m_Texture) drawData->m_Texture->UnBind();
				}

				for (auto& uniform : drawData->m_ShaderUniforms)
				{
					drawData->m_Shader->SetUniform(uniform.uniformType, uniform.name, uniform.value);
				}

				GLint glTopology = GL_TRIANGLES;
				switch (drawData->m_Topology)
				{
				case DrawData::Triangle:
					glTopology = GL_TRIANGLES;
					break;
				case DrawData::Line:
					glTopology = GL_LINES; // Note: Ensure your shader handles this primitive mode
					break;
				}

				if (drawData->m_IndicesCount == 0)
				{
					glDrawArrays(glTopology, 0, drawData->m_VerticesCount);
				}
				else if (drawData->m_IndicesCount > 0)
				{
					glDrawElements(glTopology, drawData->m_IndicesCount, GL_UNSIGNED_INT, nullptr);
				}

				drawData->m_ShaderUniforms.clear();
			}
			s_TextureSlot = 0;
		}
		// CRITICAL FIX: Empty the actual map container structure completely 
		// every single frame loop iteration pass
		s_DataToDraw.clear();
	}
}


void Renderer::SwapFrameBuffer()
{
	glfwSwapBuffers(glfwGetCurrentContext());
}
