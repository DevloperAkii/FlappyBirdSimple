#include "SpriteRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer()
{
	m_Shader = *s_ColorShader;
	m_DrawData.UpdateShader(m_Shader);

	m_Shader.Bind();
	m_Shader.SetUniform(Shader::Vec3, "u_Color", glm::vec3(1.0f, 1.0f, 1.0f));
	m_HasTextureShader = false;

	m_DrawData.GenerateDrawData(m_Vertices, m_Indices);
}

SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::Draw()
{
	// Setup matrices
	m_Shader.Bind();

	int frameBufferWidth, frameBufferHeight;
	Renderer::GetFrameBuffer(&frameBufferWidth, &frameBufferHeight);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(0.0f, (float)frameBufferWidth, 0.0f, (float)frameBufferHeight, -1.0f, 1.0f);
	m_Shader.SetUniform(Shader::Mat4, "u_ProjMat", projectionMatrix);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, Position);
	modelMatrix = glm::scale(modelMatrix, Scale);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_Shader.SetUniform(Shader::Mat4, "u_ModelMat", modelMatrix);

	Renderer::Draw(m_DrawData);
}

void SpriteRenderer::Draw(Texture& texture)
{
	m_DrawData.UpdateTexture(texture);
	if (!m_HasTextureShader)
	{
		m_Shader = *s_TextureShader;
		m_DrawData.UpdateShader(m_Shader);
		m_HasTextureShader = true;
	}

	m_Shader.SetUniform(Shader::Int, "u_BaseTexture", Renderer::GetTextureSlot());
	m_Shader.SetUniform(Shader::Vec2, "u_Tile", texture.Tile);

	Draw();
}
