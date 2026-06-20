#include "GameScene.h"
#include <print>
#include <imgui/imgui.h>

PipeSets::PipeSets(PipesType type)
{
	PipesScaleAndPositon scaleAndPos;

	scaleAndPos.Pipe_1_Position.y = 660.0f;
	scaleAndPos.Pipe_2_Position.y = -100.0f;
	m_Pipes.insert({Low,scaleAndPos});

	scaleAndPos.Pipe_1_Position.y = 820.0f;
	scaleAndPos.Pipe_2_Position.y = 60.0f;
	m_Pipes.insert({Middle,scaleAndPos});

	scaleAndPos.Pipe_1_Position.y = 960.0f;
	scaleAndPos.Pipe_2_Position.y = 200.0f;
	m_Pipes.insert({High,scaleAndPos});

	Pipe_1.Position = m_Pipes[type].Pipe_1_Position;
	Pipe_1.Scale = m_Pipes[type].Pipe_1_Scale;
	Pipe_1.Rotation = glm::vec3(180.0f, 0.0f, 0.0f);

	Pipe_2.Position = m_Pipes[type].Pipe_2_Position;
	Pipe_2.Scale = m_Pipes[type].Pipe_2_Scale;
	Pipe_2.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

PipeSets::~PipeSets()
{

}

void PipeSets::Draw(Texture& texture)
{
	Pipe_1.Position += Position;
	Pipe_2.Position += Position;

	Pipe_1.Draw(texture);
	Pipe_2.Draw(texture);
}

GameScene::GameScene(std::shared_ptr<Window>& window) : m_WindowConfig(window->m_Config)
{
	float halfWidth = m_WindowConfig.Width / 2.0f;
	float halfHeight = m_WindowConfig.Height / 2.0f;

	m_YellowBirdSprite.Position = glm::vec3(halfWidth - (halfWidth / 2), halfHeight, 0.0f);
	m_YellowBirdSprite.Scale = glm::vec3(0.2f);

	m_BackgroundSprite.Position = glm::vec3(halfWidth, halfHeight + 50.0f, 0.0f);
	m_BackgroundSprite.Scale = glm::vec3(halfWidth / 100.0f, halfHeight / 100.0f, 0.0f);

	m_BaseSprite.Position = glm::vec3(halfWidth, 50.0f, 0.0f);
	m_BaseSprite.Scale = glm::vec3(halfWidth / 100.0f, 0.5f, 1.0f);

	for (int pipeIndex = 0; pipeIndex < m_Pipes.size(); pipeIndex++)
	{
		PipeSets& pipeSet = m_Pipes[pipeIndex];
		pipeSet.Position.x = (float)m_WindowConfig.Width + (400.0f * pipeIndex);
	}
}

GameScene::~GameScene()
{
	m_Pipes.clear();
	m_BirdTextures.clear();
}

void GameScene::UpdateScene(float deltaTime)
{
	float animationSwapFrameTime = m_BirdAnimationSwapOnFrame / (float)m_AnimationFrame;
	if (m_FramePerSecond >= animationSwapFrameTime) {
		index++;
		if (index >= m_BirdTextures.size())
		{
			index = 0;
			m_Frame = 0;
		}
		m_FramePerSecond = 0.0f;
	}
	m_FramePerSecond += deltaTime;
	m_Frame++;

	m_BaseTexture.Tile += deltaTime * m_TextureScollSpeed;

	//m_BackgroundSprite.Draw(m_BackgroundTexture);

	float halfWidth = m_WindowConfig.Width / 2.0f;
	float halfHeight = m_WindowConfig.Height / 2.0f;

	/*for (auto pipeSet : m_Pipes)
	{
		pipeSet.Position.x -= deltaTime * 150.0f;
		if (pipeSet.Position.x <= -25.0f) 
		{
			float newPos = m_WindowConfig.Width + (600.0f);
			pipeSet.Position.x = newPos;
		}
		pipeSet.Draw(m_GreenPipeTexture);
	}*/

	Texture& birdTexture = m_BirdTextures[index];

	//m_BaseSprite.Draw(m_BaseTexture);
	m_YellowBirdSprite.Draw();

	//Renderer::DrawLine(glm::vec3(halfWidth - 100.0f, halfHeight, 0.0f), glm::vec3(halfWidth + 100.0f, halfHeight, 0.0f));
}
