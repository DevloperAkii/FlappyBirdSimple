#pragma once
#include "renderer/Renderer.h"
#include "renderer/Texture.h"
#include <glm/glm.hpp>

class SpriteRenderer
{
public:
	SpriteRenderer();
	SpriteRenderer(Texture* texture);
	~SpriteRenderer();

	void UpdateTexture(Texture* texture);

	DrawData* operator ->() { return &m_DrawData; }
	DrawData& operator&() { return m_DrawData;}
private:
	// Concrete items MUST be declared first in the header
	Shader m_ColorShader;
	Shader m_TextureShader;

	// Tracking pointers and data containers declared last
	Shader* m_Shader = nullptr;
	DrawData m_DrawData;
	bool m_HasTextureShader = false;
};
