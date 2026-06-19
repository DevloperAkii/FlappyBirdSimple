#pragma once
#include "SpriteRenderer.h"
#include "Window.h"

#include <memory>
#include <vector>

class GameScene 
{
public:
	GameScene(std::shared_ptr<Window>& window);
	~GameScene();

	void UpdateScene(float deltaTime);
private:
	SpriteRenderer m_BackgroundSprite;
	SpriteRenderer m_BaseSprite;
	SpriteRenderer m_YellowBirdSprite;
	SpriteRenderer m_GreenPipeSprite;

	std::vector<Texture*> m_BirdTextures;
	Texture m_BackgroundTexture = Texture(RESOURCE_PATH"Assets/sprites/background-day.png");
	Texture m_BaseTexture = Texture(RESOURCE_PATH"Assets/sprites/base.png");
	Texture m_GreenPipeTexture = Texture(RESOURCE_PATH"Assets/sprites/pipe-green.png");

	int index = 0;
	float m_Frames = 2.0f;
	float m_AnimationTimer = 0.0f;
};