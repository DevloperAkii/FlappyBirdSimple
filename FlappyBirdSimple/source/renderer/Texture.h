#pragma once
#include "PCH.h"

#include <string>
#include <glm/glm.hpp>

class Texture 
{
public:
	Texture(std::string textureFilePath);
	~Texture();

	void Bind();
	void UnBind();
	void SetTile(glm::vec2 tile);
	glm::vec2& GetTile();
private:
	friend class Renderer;

	uint32_t m_TextureID = 0;
	int m_Width = 0, m_Height = 0, m_NrComponent = 0;
	glm::vec2 m_Tile = glm::vec2(1.0f);

	inline static bool s_Init = false;
};