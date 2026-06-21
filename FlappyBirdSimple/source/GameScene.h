#pragma once
#include "SpriteRenderer.h"
#include "Window.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <miniaudio.h>

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
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
private:
    glm::mat4 GetModelMatrix();
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
    GameScene(std::shared_ptr<Window>& window, ma_engine* audioEngine);
    ~GameScene();

    void UpdateScene(float deltaTime);
private:
    bool CheckCollision(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);
private:
    WindowConfig& m_WindowConfig;

    SpriteRenderer m_BackgroundSprite;
    SpriteRenderer m_BaseSprite;
    SpriteRenderer m_YellowBirdSprite;

    // FIX 1: Store references/pointers to prevent copying and destructor deletions
    std::vector<Texture*> m_BirdTextures;

    Texture* m_RedBirdTexture = &ResourceManager::GetResource<Texture>("Red_Bird_Midflap_Texture");

    Texture* m_BackgroundTexture = nullptr;
    Texture* m_BaseTexture = nullptr;
    Texture* m_GreenPipeTexture = nullptr;

    bool m_PlayerDead = false;  
    bool m_PlayerAlive = true;  

    ma_engine* m_AudioEngine;

    std::vector<PipeSets*> m_Pipes;

    glm::vec3 m_BirdVelocity = glm::vec3(0.0f);
    float m_AngluarVelocity = 0.0f;
    bool jumpPressedBefore = false;

    float m_PipesSpeed = 100.0f;
    float m_TextureScollSpeed = 0.2f;
    int index = 0;
    const int m_AnimationFrame = 30;
    int m_BirdAnimationSwapOnFrame = 2;
    int m_Frame = 0;
    float m_FramePerSecond = 0.0f;
    float m_AnimationTimer = 0.0f;
};
