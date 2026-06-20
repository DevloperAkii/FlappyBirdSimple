#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

void DrawData::UpdateShader(Shader& shader)
{
	if (m_ShaderID) glUseProgram(0);
	m_ShaderID = shader.m_ShaderHandel;
}

void DrawData::UpdateTexture(Texture& texture)
{
	if (texture.m_TextureHandel) 
	{
		m_HasTexture = false;
		return;
	}
	if (m_TextureID) glBindTexture(GL_TEXTURE_2D, 0);
	m_TextureID = texture.m_TextureHandel;
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

		if (!s_LineData.Generated)
		{
			glGenVertexArrays(1, &s_LineData.m_VAO);
			glGenBuffers(1, &s_LineData.m_VBO);

			s_LineData.m_Shader = Shader(RESOURCE_PATH"Assets/shaders/ColorVertexShader.glsl", RESOURCE_PATH"Assets/shaders/ColorFragmentShader.glsl");
			s_LineData.Generated = true;
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

		glViewport(0, 0, s_FrameBufferWidth, s_FrameBufferHeight);
	}
}

void Renderer::GetFrameBuffer(int* width, int* height)
{
	*width = s_FrameBufferWidth;
	*height = s_FrameBufferHeight;
}

int Renderer::GetTextureSlot()
{
	return s_TextureSlot;
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
		if (drawData.m_Drawable) 
		{
			if (s_DataToDraw.contains(drawData.m_ShaderID)) 
			{
				auto& data = s_DataToDraw[drawData.m_ShaderID];
				data.push_back(drawData);
			}
			else 
			{
				std::vector<DrawData> drawDatas = {drawData};
				s_DataToDraw.insert({drawData.m_ShaderID, std::move(drawDatas)});
			}
		}
	}
}

void Renderer::DrawLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color)
{
	if (s_CurrentAPI == OPENGL)
	{
		// 1. Array contains exactly 6 floats (3 elements per point)
		float lineVertices[] =
		{
			startPos.x, startPos.y, startPos.z,
			endPos.x,   endPos.y,   endPos.z
		};

		glBindVertexArray(s_LineData.m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, s_LineData.m_VBO);

		// FIX: Changed flag to GL_DYNAMIC_DRAW since line coordinates change often
		glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_DYNAMIC_DRAW);

		// FIX: Set correct stride (3 * sizeof(float)) because there are no texture coordinates here
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// FIX: Explicitly disable attribute 1 to prevent it from reading garbage data left from previous draws
		glDisableVertexAttribArray(1);

		s_LineData.m_Shader.Bind();

		//// Set Up Orthographic Projection Matrix
		//glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)s_FrameBufferWidth, 0.0f, (float)s_FrameBufferHeight, -1.0f, 1.0f);
		//s_LineData.m_Shader.SetUniform(Shader::Mat4, "u_ProjMat", projectionMatrix);

		//// FIX: Pass a clean Identity matrix so positions are evaluated raw via absolute pixels
		//glm::mat4 modelMatrix = glm::mat4(1.0f);
		//s_LineData.m_Shader.SetUniform(Shader::Mat4, "u_ModelMat", modelMatrix);

		//// Pass primitive tracking uniforms
		//s_LineData.m_Shader.SetUniform(Shader::Vec3, "u_Color", color);

		// Render exactly 2 elements
		glDrawArrays(GL_LINES, 0, 2);

		// Clean up State
		glBindVertexArray(0);
		s_LineData.m_Shader.UnBind();
	}
}


void Renderer::EndFrame()
{
	if (s_CurrentAPI == OPENGL)
	{
		for (const auto& [id, data] : s_DataToDraw)
		{
			for (const auto& drawData : data) {

				glBindVertexArray(drawData.m_VAO);
				glUseProgram(drawData.m_ShaderID);

				// Isolate texture bindings per entity
				if (drawData.m_HasTexture)
				{
					glActiveTexture(GL_TEXTURE0 + s_TextureSlot); // Lock to binding slot 0
					glBindTexture(GL_TEXTURE_2D, drawData.m_TextureID);
					s_TextureSlot++;
				}
				else
				{
					// FIX: If the shape doesn't use a texture, break the bind on slot 0
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}

				glDrawElements(GL_TRIANGLES, drawData.m_IndicesCount, GL_UNSIGNED_INT, nullptr);
				glUseProgram(0);
			}
			s_TextureSlot = 0;
		}
		// CRITICAL FIX: Empty the draw list queue container completely before starting a new frame
		s_DataToDraw.clear();
	}
}

void Renderer::SwapFrameBuffer()
{
	glfwSwapBuffers(glfwGetCurrentContext());
}
