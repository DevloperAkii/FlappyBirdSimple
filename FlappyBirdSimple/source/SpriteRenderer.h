#pragma once
#include "renderer/Renderer.h"
#include "renderer/Texture.h"
#include "ResourceManager.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class SpriteRenderer
{
public:
    SpriteRenderer();
    ~SpriteRenderer() = default;

    void Draw();
    void Draw(Texture& texture);
public:
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
private:
    Shader* m_ColorShader = nullptr;
    Shader* m_TextureShader = nullptr;
    Shader* m_Shader = nullptr; // Track using a assignable pointer

    std::vector<float> m_Vertices = {
         100.0f, -100.0f, 0.0f,     1.0f, 0.0f,
        -100.0f, -100.0f, 0.0f,     0.0f, 0.0f,
        -100.0f,  100.0f, 0.0f,     0.0f, 1.0f,
         100.0f,  100.0f, 0.0f,     1.0f, 1.0f,
    };
    std::vector<uint32_t> m_Indices = { 0, 1, 2, 2, 3, 0 };

    DrawData m_DrawData;
    bool m_HasTextureShader = false;
};

