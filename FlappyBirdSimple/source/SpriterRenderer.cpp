#include "SpriteRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer() :
	m_ColorShader(RESOURCE_PATH"Assets/shaders/ColorVertexShader.glsl", RESOURCE_PATH"Assets/shaders/ColorFragmentShader.glsl"),
	m_TextureShader(RESOURCE_PATH"Assets/shaders/TextureVertexShader.glsl", RESOURCE_PATH"Assets/shaders/TextureFragmentShader.glsl")
{
	m_Shader = &m_ColorShader;
	m_DrawData.UpdateShader(m_Shader); // Fix the nullptr assignment

	std::vector<float> vertices = {
		100.0f, -100.0f, 0.0f,		1.0f, 0.0f,
	   -100.0f, -100.0f, 0.0f,		0.0f, 0.0f,
	   -100.0f,  100.0f, 0.0f,		0.0f, 1.0f,
		100.0f,  100.0f, 0.0f,		1.0f, 1.0f,
	};
	std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
	m_DrawData.GenerateDrawData(vertices, indices);

	m_Shader->Bind();
	m_Shader->SetUniform(Shader::Vec3, "u_Color", glm::vec3(1.0f, 1.0f, 1.0f));
	m_HasTextureShader = false;
}

SpriteRenderer::SpriteRenderer(Texture* texture) :
	m_ColorShader(RESOURCE_PATH"Assets/shaders/ColorVertexShader.glsl", RESOURCE_PATH"Assets/shaders/ColorFragmentShader.glsl"),
	m_TextureShader(RESOURCE_PATH"Assets/shaders/TextureVertexShader.glsl", RESOURCE_PATH"Assets/shaders/TextureFragmentShader.glsl")
{
	m_Shader = &m_TextureShader;
	m_DrawData.UpdateShader(m_Shader); // Fix the nullptr assignment
	m_DrawData.UpdateTexture(texture);

	std::vector<float> vertices = {
		100.0f, -100.0f, 0.0f,		1.0f, 0.0f,
	   -100.0f, -100.0f, 0.0f,		0.0f, 0.0f,
	   -100.0f,  100.0f, 0.0f,		0.0f, 1.0f,
		100.0f,  100.0f, 0.0f,		1.0f, 1.0f,
	};
	std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
	m_DrawData.GenerateDrawData(vertices, indices);

	m_Shader->Bind();
	m_Shader->SetUniform(Shader::Vec3, "u_Color", glm::vec3(1.0f, 1.0f, 1.0f));
	m_HasTextureShader = true;
}


SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::UpdateTexture(Texture* texture)
{
	if (!texture) 
	{
		m_HasTextureShader = false;
		return;
	}
	m_DrawData.UpdateTexture(texture);
	if (!m_HasTextureShader) 
	{
		m_Shader = &m_TextureShader;
		m_DrawData.UpdateShader(m_Shader);

		m_Shader->Bind();
		m_Shader->SetUniform(Shader::Vec3, "u_Color", glm::vec3(1.0f, 1.0f, 1.0f));
		m_HasTextureShader = true;
	}
}
