#pragma once
#include "SpriteRenderer.h"
#include "Window.h"

#include <memory>
#include <vector>
#include <unordered_map>

struct PipeSets 
{
	enum PipesType 
	{
		Low = 0,
		Middle = 1,
		High = 2,
	};

	PipeSets(PipesType type);
	~PipeSets();

	void Draw(Texture& texture);

	SpriteRenderer Pipe_1;
	SpriteRenderer Pipe_2;

	glm::vec3 Position = glm::vec3(0.0f);
private:
	struct PipesScaleAndPositon
	{
		glm::vec3 Pipe_1_Position = glm::vec3(0.0f), Pipe_1_Scale = glm::vec3(0.4f, 3.0f, 0.0f);
		glm::vec3 Pipe_2_Position = glm::vec3(0.0f), Pipe_2_Scale = glm::vec3(0.4f, 3.0f, 0.0f);
	};
	std::unordered_map<PipesType, PipesScaleAndPositon> m_Pipes = std::unordered_map<PipesType, PipesScaleAndPositon>();
};

class GameScene 
{
public:
	GameScene(std::shared_ptr<Window>& window);
	~GameScene();

	void UpdateScene(float deltaTime);
private:
	WindowConfig& m_WindowConfig;

	SpriteRenderer m_BackgroundSprite;
	SpriteRenderer m_BaseSprite;
	SpriteRenderer m_YellowBirdSprite;

	std::vector<Texture> m_BirdTextures = 
	{
		Texture(RESOURCE_PATH"Assets/sprites/yellowbird-upflap.png"),
		Texture(RESOURCE_PATH"Assets/sprites/yellowbird-midflap.png"),
		Texture(RESOURCE_PATH"Assets/sprites/yellowbird-downflap.png"),
	};
	Texture m_BackgroundTexture = Texture(RESOURCE_PATH"Assets/sprites/background-day.png");
	Texture m_BaseTexture = Texture(RESOURCE_PATH"Assets/sprites/base.png");
	Texture m_GreenPipeTexture = Texture(RESOURCE_PATH"Assets/sprites/pipe-green.png");

	std::vector<PipeSets> m_Pipes = 
	{
		PipeSets(PipeSets::Low),
		PipeSets(PipeSets::Middle),
		PipeSets(PipeSets::High),
	};
	//PipeSets m_ConfigurationPipes = PipeSets(PipeSets::High, m_GreenPipeTexture);

	//Base Animation Controls
	float m_TextureScollSpeed = 0.2f;

	//Brid Animation Controls
	int index = 0;
	const int m_AnimationFrame = 30;

	int m_BirdAnimationSwapOnFrame = 2;
	int m_Frame = 0;

	float m_FramePerSecond = 0.0f;
	float m_AnimationTimer = 0.0f;
};