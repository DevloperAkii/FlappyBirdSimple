#include "GameScene.h"
#include <print>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

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
    
    Position = glm::vec3(0.0f);
}

glm::mat4 PipeSets::GetModelMatrix()
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return translation * scale * rotation;
}

GameScene::GameScene(std::shared_ptr<Window>& window, ma_engine* audioEngine)
    : m_WindowConfig(window->m_Config), m_AudioEngine(audioEngine)
{
    // FIX 2: Bind pointers directly to the resources
    m_BackgroundTexture = &ResourceManager::GetResource<Texture>("Background_Texture");
    m_BaseTexture = &ResourceManager::GetResource<Texture>("Base_Texture");
    m_GreenPipeTexture = &ResourceManager::GetResource<Texture>("Pipe_Texture");

    m_BirdTextures.push_back(&ResourceManager::GetResource<Texture>("Yellow_Bird_Upflap_Texture"));
    m_BirdTextures.push_back(&ResourceManager::GetResource<Texture>("Yellow_Bird_Middleflap_Texture"));
    m_BirdTextures.push_back(&ResourceManager::GetResource<Texture>("Yellow_Bird_Downflap_Texture"));

    float halfWidth = m_WindowConfig.Width / 2.0f;
    float halfHeight = m_WindowConfig.Height / 2.0f;

    m_YellowBirdSprite.Position = glm::vec3(halfWidth - (halfWidth / 2), halfHeight, 0.0f);
    m_YellowBirdSprite.Scale = glm::vec3(0.2f);

    m_BackgroundSprite.Position = glm::vec3(halfWidth, halfHeight + 50.0f, 0.0f);
    m_BackgroundSprite.Scale = glm::vec3(halfWidth / 100.0f, halfHeight / 100.0f, 0.0f);

    m_BaseSprite.Position = glm::vec3(halfWidth, 50.0f, 0.0f);
    m_BaseSprite.Scale = glm::vec3(halfWidth / 100.0f, 0.5f, 1.0f);

    m_Pipes.push_back(new PipeSets(PipeSets::Low));
    m_Pipes.push_back(new PipeSets(PipeSets::Middle));
    m_Pipes.push_back(new PipeSets(PipeSets::High));

    for (int pipeIndex = 0; pipeIndex < m_Pipes.size(); pipeIndex++)
    {
        PipeSets& pipeSet = *m_Pipes[pipeIndex];
        pipeSet.Position.x = (float)m_WindowConfig.Width + (400.0f * pipeIndex);
    }
}

GameScene::~GameScene()
{
    for (auto pipeSet : m_Pipes) delete pipeSet;
    m_Pipes.clear();
    m_BirdTextures.clear(); // Safe now, vectors store pointers, no destructors run on textures!
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

    // FIX 3: Update tracking states directly through the bound pointer
    m_BaseTexture->Tile.x += deltaTime * m_TextureScollSpeed;

    // FIX 4: Explicitly pass your background texture to trip the texture shader setup
    m_BackgroundSprite.Draw(*m_BackgroundTexture);

    float halfWidth = m_WindowConfig.Width / 2.0f;
    float halfHeight = m_WindowConfig.Height / 2.0f;

    Texture* birdTexture = m_BirdTextures[index];
    for (auto pipeSet : m_Pipes) 
    {
        pipeSet->Position.x -= deltaTime * m_PipesSpeed;
        if (pipeSet->Pipe_1.Position.x < -50.0f) 
        {
            pipeSet->Pipe_1.Position.x = m_Pipes[m_Pipes.size() - 1]->Position.x + 1200.0f;
            pipeSet->Pipe_2.Position.x = m_Pipes[m_Pipes.size() - 1]->Position.x + 1200.0f;

            std::rotate(m_Pipes.begin(), m_Pipes.begin() + 1, m_Pipes.end());
        }
        pipeSet->Draw(*m_GreenPipeTexture);

        if (CheckCollision(m_YellowBirdSprite.Position, m_YellowBirdSprite.Scale * 200.0f, pipeSet->Pipe_1.Position, pipeSet->Pipe_1.Scale * 200.0f))
        {
            m_PlayerDead = true;
            if (m_PlayerAlive)
            {
                int result = ma_engine_play_sound(m_AudioEngine, RESOURCE_PATH"Assets/audio/swoosh.wav", NULL);
                if (result != MA_SUCCESS) {
                    std::println("Error playing sound: {}{}", result, RESOURCE_PATH"Assets/audio/wing.wav");
                }
            }
        }
        if (CheckCollision(m_YellowBirdSprite.Position, m_YellowBirdSprite.Scale * 200.0f, pipeSet->Pipe_2.Position, pipeSet->Pipe_2.Scale * 200.0f))
        {
            m_PlayerDead = true;
        }
    }

    // Dereference the pointer handle safely to send it off to draw

    auto io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    float constGravity = -400.0f;
    m_BirdVelocity.y += constGravity * deltaTime;
    m_AngluarVelocity -= 150.0f * deltaTime;

    bool jumpPressedNow = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS;

    if (!jumpPressedBefore && jumpPressedNow) 
    {
        m_BirdVelocity.y = 120.0f;
        m_AngluarVelocity = glm::sqrt(150.0f * 2 * 45.0f);
        int result = ma_engine_play_sound(m_AudioEngine, RESOURCE_PATH"Assets/audio/wing.wav", NULL);
        if (result != MA_SUCCESS) {
            std::println("Error playing sound: {}{}", result, RESOURCE_PATH"Assets/audio/wing.wav");
        }
    }

    if ((m_YellowBirdSprite.Rotation.z < -90.0f && m_AngluarVelocity < 0) || (m_YellowBirdSprite.Rotation.z > 45.0f && m_AngluarVelocity > 0)) 
    {
        m_AngluarVelocity = 0.0f;
    }

    jumpPressedBefore = jumpPressedNow;

    if (CheckCollision(m_YellowBirdSprite.Position, m_YellowBirdSprite.Scale * 200.0f, m_BaseSprite.Position, m_BaseSprite.Scale * 200.0f))
    {
        m_BirdVelocity = glm::vec3(0.0f);
        m_PlayerDead = true;
    }

    if (m_PlayerDead) 
    {
        birdTexture = m_RedBirdTexture;
        if (m_PlayerAlive) 
        {
            int result = ma_engine_play_sound(m_AudioEngine, RESOURCE_PATH"Assets/audio/hit.wav", NULL);
            result = ma_engine_play_sound(m_AudioEngine, RESOURCE_PATH"Assets/audio/die.wav", NULL);
            if (result != MA_SUCCESS) {
                std::println("Error playing sound: {}{}", result, RESOURCE_PATH"Assets/audio/wing.wav");
            }
        }
        m_PipesSpeed = 0.0f;
        m_PlayerAlive = false;
    }

    m_YellowBirdSprite.Position += m_BirdVelocity * deltaTime;
    m_YellowBirdSprite.Rotation += glm::vec3(0.0f, 0.0f, 1.0f) * m_AngluarVelocity * deltaTime;
    m_BaseSprite.Draw(*m_BaseTexture);
    m_YellowBirdSprite.Draw(*birdTexture);
}

bool GameScene::CheckCollision(const glm::vec2& center1, const glm::vec2& size1,
    const glm::vec2& center2, const glm::vec2& size2) {

    // Calculate the distance between centers
    float deltaX = glm::abs(center1.x - center2.x);
    float deltaY = glm::abs(center1.y - center2.y);

    // Collision occurs if distance is less than half the combined sizes
    bool collideX = deltaX <= (size1.x / 2.0f + size2.x / 2.0f);
    bool collideY = deltaY <= (size1.y / 2.0f + size2.y / 2.0f);

    return collideX && collideY;
}

