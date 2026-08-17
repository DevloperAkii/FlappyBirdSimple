#include "GameScene.h"
#include "game/Physics.h"
#include "Application.h"

#include <print>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

GameScene::GameScene(std::shared_ptr<Window>& window, ma_engine* audioEngine, Application* app)
    : m_WindowConfig(window->m_Config), m_AudioEngine(audioEngine), m_App(app), m_Pipes(m_WindowConfig), m_Bird(m_WindowConfig, m_AudioEngine),
    m_BaseTile(m_WindowConfig)
{
    // FIX 2: Bind pointers directly to the resources
    m_BackgroundTexture = &ResourceManager::GetResource<Texture>("Background_Texture");

    float halfWidth = m_WindowConfig.Width / 2.0f;
    float halfHeight = m_WindowConfig.Height / 2.0f;

    m_BackgroundSprite.Position = glm::vec3(halfWidth, halfHeight + 50.0f, 0.0f);
    m_BackgroundSprite.Scale = glm::vec3(halfWidth / 100.0f, halfHeight / 100.0f, 0.0f);

    ImGuiIO& io = ImGui::GetIO();
    // Load your default font first
    io.Fonts->AddFontDefault();

    ImFontConfig config;
    config.SizePixels = 60.0f; // This is equivalent to a massive scale natively
    m_ScoreFont = io.Fonts->AddFontDefault(&config);
    config.SizePixels = 35.0f; // This is equivalent to a massive scale natively
    m_MessageFont = io.Fonts->AddFontDefault(&config);
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
        m_Pipes.SetSpeed(0.0f);
        m_BaseTile.SetScrollSpeed(0.0f);
        m_PlayerDead = true;
    }

    if (m_Pipes.CheckCollision(m_Bird.Position, m_Bird.Scale))
    {
        if (m_PlayerAlive)
        {
            PlaySoundEffect(RESOURCE_PATH"Assets/audio/swoosh.wav");
            m_Pipes.SetSpeed(0.0f);
            m_BaseTile.SetScrollSpeed(0.0f);
        }
        m_PlayerDead = true;
    }

    int collidedPipeIndex = 0;
    if (m_Pipes.CheckBirdCrossing(m_Bird.Position, collidedPipeIndex)) 
    {
        if (collidedPipeIndex != m_LastCollidedPipeIndex) 
        {
            m_Score++;
            m_LastCollidedPipeIndex = collidedPipeIndex;
            PlaySoundEffect(RESOURCE_PATH"Assets/audio/point.wav");
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

    if (!m_GameStarted && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS) m_GameStarted = true;

    if (m_GameStarted) 
    {
        m_Bird.Update(deltaTime);
        m_Pipes.Update(deltaTime);
        m_BaseTile.Update(deltaTime);
    }

    m_BackgroundSprite.Draw(*m_BackgroundTexture);
    m_Pipes.Draw();
    m_BaseTile.Draw();
    m_Bird.Draw();

    if (m_PlayerDead && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R) == GLFW_PRESS)
    {
        m_App->Restart();
    }
}

void GameScene::GuiScene()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollbar; // Recommended for clean overlays

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (!m_GameStarted)
    {
        std::string message = "Press 'Space' To Start";

        // 1. Get exact text size using the correct font
        ImGui::PushFont(m_MessageFont);
        ImVec2 textSize = ImGui::CalcTextSize(message.c_str());
        ImGui::PopFont();

        // 2. Center the window perfectly by subtracting half the text size from the screen center
        float posX = (m_WindowConfig.Width / 2.0f)  - (textSize.x / 2.0f);
        float posY = (m_WindowConfig.Height / 2.0f) - (textSize.y / 2.0f) + 50.0f;

        // 3. Change flag to Always to override old saved .ini values
        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);
        // Give the window a unique matching size so text doesn't wrap/clip
        ImGui::SetNextWindowSize(textSize, ImGuiCond_Always);

        ImGui::Begin("Start Window", nullptr, window_flags);
        ImGui::PushFont(m_MessageFont);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        ImGui::Text(message.c_str());
        ImGui::PopStyleColor();

        ImGui::PopFont();
        ImGui::End();
    }
    if (!m_PlayerDead && m_GameStarted)
    {
        // 1. Get exact size of score text
        char score_str[32];
        snprintf(score_str, sizeof(score_str), "%.0f", (float)m_Score);

        ImGui::PushFont(m_ScoreFont);
        ImVec2 scoreSize = ImGui::CalcTextSize(score_str);
        ImGui::PopFont();

        // 2. Align center X, place at Y=100
        float posX = (m_WindowConfig.Width / 2.0f) - (scoreSize.x / 2.0f);

        ImGui::SetNextWindowPos(ImVec2(posX, 100.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(scoreSize, ImGuiCond_Always);

        ImGui::Begin("Score Window", nullptr, window_flags);
        ImGui::PushFont(m_ScoreFont);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        ImGui::Text("%s", score_str);
        ImGui::PopStyleColor();

        ImGui::PopFont();
        ImGui::End();
    }
    else if (m_GameStarted)
    {
        std::string message = "Press 'R' To Restart";

        ImGui::PushFont(m_MessageFont);
        ImVec2 textSize = ImGui::CalcTextSize(message.c_str());
        ImGui::PopFont();

        float posX = (m_WindowConfig.Width / 2.0f)  - (textSize.x / 2.0f);
        float posY = (m_WindowConfig.Height / 2.0f) - (textSize.y / 2.0f);

        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);
        ImGui::SetNextWindowSize(textSize, ImGuiCond_Always);

        ImGui::Begin("Restart Window", nullptr, window_flags);
        ImGui::PushFont(m_MessageFont);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        ImGui::Text(message.c_str());
        ImGui::PopStyleColor();

        ImGui::PopFont();
        ImGui::End();
    }

    ImGui::PopStyleVar();
}

void GameScene::PlaySoundEffect(std::string filePath)
{
    int result = ma_engine_play_sound(m_AudioEngine, filePath.c_str(), NULL);
    if (result != MA_SUCCESS) std::println("Error playing sound: {}{}", result, RESOURCE_PATH"Assets/audio/wing.wav");
}
