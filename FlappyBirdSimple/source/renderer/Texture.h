#pragma once
#include "PCH.h"
#include "Shader.h"

#include <string>
#include <glm/glm.hpp>

class Texture 
{
public:
	Texture(std::string textureFilePath);
	~Texture();

	void Bind();
	void UnBind();
public:
	glm::vec2 Tile = glm::vec2(1.0f);
private:
	friend class Renderer;
	friend struct DrawData;

	uint32_t m_TextureHandel = 0;
	int m_Width = 0, m_Height = 0, m_NrComponent = 0;

	inline static bool s_Init = false;
};