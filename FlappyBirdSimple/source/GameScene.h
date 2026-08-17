#pragma once
#include "SpriteRenderer.h"
#include "Window.h"
#include "game/Bird.h"
#include "game/Pipes.h"
#include "game/BaseTile.h"

#include <imgui/imgui.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <miniaudio.h>

class Application;

class GameScene
{
public:
    GameScene(std::shared_ptr<Window>& window, ma_engine* audioEngine, Application* app);
    ~GameScene();

    void UpdateScene(float deltaTime);
    void GuiScene();
private:
    void PlaySoundEffect(std::string filePath);
private:
    WindowConfig& m_WindowConfig;
    ma_engine* m_AudioEngine = nullptr;
    Application* m_App = nullptr;

    ImFont* m_ScoreFont  = nullptr;
    ImFont* m_MessageFont = nullptr;

    SpriteRenderer m_BackgroundSprite;
    Pipes m_Pipes;
    Bird m_Bird;
    BaseTile m_BaseTile;

    Texture* m_BackgroundTexture = nullptr;

    bool m_PlayerDead = false;  
    bool m_PlayerAlive = true;  
    bool m_GameStarted = false;

    int m_LastCollidedPipeIndex = -1;
    int m_Score = 0;
};
