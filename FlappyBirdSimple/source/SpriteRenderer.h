#pragma once
#include "renderer/Renderer.h"
#include "renderer/Texture.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class SpriteRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void Draw();
	void Draw(Texture& texture);
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
private:
	// Concrete items MUST be declared first in the header
	inline static std::shared_ptr<Shader> s_ColorShader 
		= std::make_shared<Shader>(RESOURCE_PATH"Assets/shaders/ColorVertexShader.glsl", RESOURCE_PATH"Assets/shaders/ColorFragmentShader.glsl");
	inline static std::shared_ptr<Shader> s_TextureShader
		= std::make_shared<Shader>(RESOURCE_PATH"Assets/shaders/TextureVertexShader.glsl", RESOURCE_PATH"Assets/shaders/TextureFragmentShader.glsl");

	std::vector<float> m_Vertices = {
		 100.0f, -100.0f, 0.0f,		1.0f, 0.0f,
		-100.0f, -100.0f, 0.0f,		0.0f, 0.0f,
		-100.0f,  100.0f, 0.0f,		0.0f, 1.0f,
		 100.0f,  100.0f, 0.0f,		1.0f, 1.0f,
	};
	std::vector<uint32_t> m_Indices = { 0, 1, 2, 2, 3, 0 };

	// Tracking pointers and data containers declared last
	Shader& m_Shader = *s_ColorShader;
	DrawData m_DrawData;
	bool m_HasTextureShader = false;
};
