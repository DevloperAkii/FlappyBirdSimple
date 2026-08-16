#pragma once
#include "SpriteRenderer.h"
#include "Window.h"
#include "game/Bird.h"
#include "game/Pipes.h"
#include "game/BaseTile.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <miniaudio.h>


class GameScene
{
public:
    GameScene(std::shared_ptr<Window>& window, ma_engine* audioEngine);
    ~GameScene();

    void UpdateScene(float deltaTime);
private:
    void PlaySoundEffect(std::string filePath);
private:
    WindowConfig& m_WindowConfig;
    ma_engine* m_AudioEngine;

    SpriteRenderer m_BackgroundSprite;

    Pipes m_Pipes = Pipes(m_WindowConfig);
    Bird m_Bird = Bird(m_WindowConfig, m_AudioEngine);
    BaseTile m_BaseTile = BaseTile(m_WindowConfig);

    Texture* m_BackgroundTexture = nullptr;

    bool m_PlayerDead = false;  
    bool m_PlayerAlive = true;  
};
