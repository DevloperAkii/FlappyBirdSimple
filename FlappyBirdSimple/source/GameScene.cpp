#include "GameScene.h"
#include <print>

GameScene::GameScene(std::shared_ptr<Window>& window)
{
	m_BirdTextures.push_back(new Texture(RESOURCE_PATH"Assets/sprites/yellowbird-upflap.png"));
	m_BirdTextures.push_back(new Texture(RESOURCE_PATH"Assets/sprites/yellowbird-midflap.png"));
	m_BirdTextures.push_back(new Texture(RESOURCE_PATH"Assets/sprites/yellowbird-downflap.png"));

	float halfWidth = window->m_Config.Width / 2.0f;
	float halfHeight = window->m_Config.Height / 2.0f;

	m_YellowBirdSprite->Position = glm::vec3(halfWidth - (halfWidth / 2), halfHeight, 0.0f);
	m_YellowBirdSprite->Scale = glm::vec3(0.2f);
	m_YellowBirdSprite.UpdateTexture(m_BirdTextures[index]);

	m_BackgroundSprite->Position = glm::vec3(halfWidth, halfHeight + 50.0f, 0.0f);
	m_BackgroundSprite->Scale = glm::vec3(halfWidth / 100.0f, halfHeight / 100.0f, 0.0f);
	m_BackgroundSprite.UpdateTexture(&m_BackgroundTexture);

	m_BaseSprite->Position = glm::vec3(halfWidth, 50.0f, 0.0f);
	m_BaseSprite->Scale = glm::vec3(halfWidth / 100.0f, 0.5f, 1.0f);
	m_BaseSprite.UpdateTexture(&m_BaseTexture);

	m_GreenPipeSprite->Position = glm::vec3(halfWidth + (halfWidth / 2), 170.0f, 0.0f);
	m_GreenPipeSprite->Scale = glm::vec3(0.4, 1.2, 0.0f);
	m_GreenPipeSprite.UpdateTexture(&m_GreenPipeTexture);
}

GameScene::~GameScene()
{
	for (size_t i = 0; i < m_BirdTextures.size(); ++i)
	{
		delete m_BirdTextures[i];
	}
	m_BirdTextures.clear();
}

void GameScene::UpdateScene(float deltaTime)
{
	m_AnimationTimer += deltaTime;
	if (m_AnimationTimer >= m_Frames) 
	{
		m_AnimationTimer = 0.0f;
	}
	index++;
	if (index >= m_BirdTextures.size())
	{
		index = 0;
	}
	m_YellowBirdSprite.UpdateTexture(m_BirdTextures[index]);

	glm::vec2 currentTile = m_BaseTexture.GetTile();
	currentTile.x += deltaTime;
	m_BaseTexture.SetTile(currentTile);

	Renderer::Draw(&m_BackgroundSprite);
	Renderer::Draw(&m_GreenPipeSprite);
	Renderer::Draw(&m_BaseSprite);
	Renderer::Draw(&m_YellowBirdSprite);
}
