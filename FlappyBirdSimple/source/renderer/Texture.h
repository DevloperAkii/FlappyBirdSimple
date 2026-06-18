#pragma once
#include <string>
#include "PCH.h"

//TODO : Should managed by a Resource Manager
static uint32_t s_TexturesCount = 0;

class Texture 
{
public:
	Texture(std::string textureFilePath);
	~Texture();

	void Bind();
	void UnBind();
private:
	friend class Renderer;

	uint32_t m_TextureID = 0;
	int m_Width = 0, m_Height = 0, m_NrComponent = 0;
	int m_TextureSlot = -1;
	bool m_Bounded = false;

	inline static bool s_Init = false;
};