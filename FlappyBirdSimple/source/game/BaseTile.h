#pragma once
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Physics.h"

class BaseTile
{
public:
	BaseTile(WindowConfig& windowConfig) 
	{
		float halfWidth  = windowConfig.Width / 2.0f;
		float halfHeight = windowConfig.Height / 2.0f;

		Position = glm::vec3(halfWidth, 50.0f, 0.0f);
		Scale    = glm::vec3(halfWidth / 100.0f, 0.5f, 1.0f);

		m_BaseSprite.Position = Position;
		m_BaseSprite.Scale    = Scale;
	}
	~BaseTile() 
	{

	}

	void Update(float deltaTime) 
	{
		m_BaseTexture->Tile.x += deltaTime * m_TextureScollSpeed;
	}

	void Draw() 
	{
		m_BaseSprite.Draw(*m_BaseTexture);
	}

	bool CheckCollision(glm::vec3 Position, glm::vec3 Scale) 
	{
		if (Physics::CheckCollision(Position, Scale * 200.0f, this->Position, this->Scale * 200.0f))
		{
			return true;
		}
		return false;
	}

	inline void SetScrollSpeed(float speed) {m_TextureScollSpeed = speed;}
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale    = glm::vec3(1.0f);
private:
	SpriteRenderer m_BaseSprite;
	Texture* m_BaseTexture = &ResourceManager::GetResource<Texture>("Base_Texture");

	float m_TextureScollSpeed = 0.2f;
};