#pragma once

#include "PCH.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

#include <vector>
#include <GLFW/glfw3.h>
#include <miniaudio.h>

class Bird 
{
public:
    Bird(WindowConfig& windowConfig, ma_engine* audioEngine) : m_AudioEngine(audioEngine)
    {
        m_BirdTextures.push_back(&ResourceManager::GetResource<Texture>("Yellow_Bird_Upflap_Texture"));
        m_BirdTextures.push_back(&ResourceManager::GetResource<Texture>("Yellow_Bird_Middleflap_Texture"));
        m_BirdTextures.push_back(&ResourceManager::GetResource<Texture>("Yellow_Bird_Downflap_Texture"));

        float halfWidth  = windowConfig.Width  / 2.0f;
        float halfHeight = windowConfig.Height / 2.0f;

        Position = glm::vec3(halfWidth - (halfWidth / 2), halfHeight, 0.0f);
        Scale    = glm::vec3(0.2f);

        m_YellowBirdSprite.Position = Position;
        m_YellowBirdSprite.Scale    = Scale;

        m_GameOver = false;
    }

    ~Bird() 
    {
        m_BirdTextures.clear(); 
    }

    inline void Update(float deltaTime) 
    {
        float constGravity = -400.0f;
        m_BirdVelocity.y += constGravity * deltaTime;
        m_AngluarVelocity -= 120.0f * deltaTime;

        bool jumpPressedNow = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS;

        if (!jumpPressedBefore && jumpPressedNow && !m_GameOver)
        {
            m_BirdVelocity.y = 120.0f;
            m_AngluarVelocity = 0.0f;
            Rotation.z = 45.0f;
            int result = ma_engine_play_sound(m_AudioEngine, RESOURCE_PATH"Assets/audio/wing.wav", NULL);
            if (result != MA_SUCCESS) {
                std::println("Error playing sound: {}{}", result, RESOURCE_PATH"Assets/audio/wing.wav");
            }
        }

        jumpPressedBefore = jumpPressedNow;

        if ((Rotation.z < -75.0f && m_AngluarVelocity < 0))
        {
            m_AngluarVelocity = 0.0f;
        }

        float animationSwapFrameTime = m_BirdAnimationSwapOnFrame / (float)c_AnimationFrame;
        if (m_FramePerSecond >= animationSwapFrameTime) {
            m_BirdTextureIndex++;
            if (m_BirdTextureIndex >= m_BirdTextures.size())
            {
                m_BirdTextureIndex = 0;
                m_Frame = 0;
            }
            m_FramePerSecond = 0.0f;
        }
        m_FramePerSecond += deltaTime;
        m_Frame++;

        Position += m_BirdVelocity * deltaTime;
        Rotation += glm::vec3(0.0f, 0.0f, 1.0f) * m_AngluarVelocity * deltaTime;

        m_YellowBirdSprite.Position = Position;
        m_YellowBirdSprite.Rotation = Rotation;
    }
    inline void Draw() 
    {
        Texture* birdTexture = m_BirdTextures[m_BirdTextureIndex];
        if (m_GameOver) birdTexture = m_RedBirdTexture;
        m_YellowBirdSprite.Draw(*birdTexture);
    }
    inline void SetVelocity(glm::vec3 velocity) {m_BirdVelocity = velocity;}
	inline void SetGameOver(bool gameover) {m_GameOver = gameover;}
public:
    glm::vec3 Position = glm::vec3(1.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale    = glm::vec3(1.0f);
private:
    const int c_AnimationFrame = 30;

    int m_BirdAnimationSwapOnFrame = 2;
    int m_Frame = 0;
    int m_BirdTextureIndex = 0;

    float m_FramePerSecond = 0.0f;
    float m_AnimationTimer = 0.0f;

    bool jumpPressedBefore = false;
    bool m_GameOver = false;

    glm::vec3 m_BirdVelocity = glm::vec3(0.0f);
    float m_AngluarVelocity = 0.0f;

    std::vector<Texture*> m_BirdTextures;
    Texture* m_RedBirdTexture = &ResourceManager::GetResource<Texture>("Red_Bird_Midflap_Texture");

    SpriteRenderer m_YellowBirdSprite;
    ma_engine* m_AudioEngine;
};