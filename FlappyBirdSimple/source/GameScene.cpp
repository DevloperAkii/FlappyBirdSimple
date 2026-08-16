#include "GameScene.h"
#include "game/Physics.h"

#include <print>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

GameScene::GameScene(std::shared_ptr<Window>& window, ma_engine* audioEngine)
    : m_WindowConfig(window->m_Config), m_AudioEngine(audioEngine)
{
    // FIX 2: Bind pointers directly to the resources
    m_BackgroundTexture = &ResourceManager::GetResource<Texture>("Background_Texture");

    float halfWidth = m_WindowConfig.Width / 2.0f;
    float halfHeight = m_WindowConfig.Height / 2.0f;

    m_BackgroundSprite.Position = glm::vec3(halfWidth, halfHeight + 50.0f, 0.0f);
    m_BackgroundSprite.Scale = glm::vec3(halfWidth / 100.0f, halfHeight / 100.0f, 0.0f);
}

GameScene::~GameScene()
{
}

void GameScene::UpdateScene(float deltaTime)
{
    //auto io = ImGui::GetIO();
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    if (m_BaseTile.CheckCollision(m_Bird.Position, m_Bird.Scale))
    {
        m_Bird.SetVelocity(glm::vec3(0.0f));
        m_PlayerDead = true;
    }

    if (m_Pipes.CheckCollision(m_Bird.Position, m_Bird.Scale))
    {
        m_PlayerDead = true;
        if (m_PlayerAlive)
        {
            PlaySoundEffect(RESOURCE_PATH"Assets/audio/swoosh.wav");
            m_Pipes.SetSpeed(0.0f);
        }
    }

    if (m_PlayerDead) 
    {
        m_Bird.SetGameOver(true);
        if (m_PlayerAlive) 
        {
            PlaySoundEffect(RESOURCE_PATH"Assets/audio/hit.wav");
            PlaySoundEffect(RESOURCE_PATH"Assets/audio/die.wav");
        }
        m_PlayerAlive = false;
    }

    m_Bird.Update(deltaTime);
    m_Pipes.Update(deltaTime);
    m_BaseTile.Update(deltaTime);

    m_BackgroundSprite.Draw(*m_BackgroundTexture);
    m_Pipes.Draw();
    m_BaseTile.Draw();
    m_Bird.Draw();
}

void GameScene::PlaySoundEffect(std::string filePath)
{
    int result = ma_engine_play_sound(m_AudioEngine, filePath.c_str(), NULL);
    if (result != MA_SUCCESS) {
        std::println("Error playing sound: {}{}", result, RESOURCE_PATH"Assets/audio/wing.wav");
    }
}
