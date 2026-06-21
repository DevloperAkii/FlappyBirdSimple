#include "SpriteRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer()
{
    m_ColorShader = &ResourceManager::GetResource<Shader>("Color_Shader");
    m_TextureShader = &ResourceManager::GetResource<Shader>("Texture_Shader");

    m_Shader = m_ColorShader;
    m_DrawData.UpdateShader(*m_Shader);
    m_DrawData.GenerateDrawData(m_Vertices, m_Indices);
}

void SpriteRenderer::Draw()
{
    int frameBufferWidth, frameBufferHeight;
    Renderer::GetFrameBuffer(&frameBufferWidth, &frameBufferHeight);

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)frameBufferWidth, 0.0f, (float)frameBufferHeight, -1.0f, 1.0f);
    m_DrawData.UpdateUniform(Shader::Mat4, "u_ProjMat", projectionMatrix);

    glm::mat4 modelMatrix = GetModelMatrix();
    m_DrawData.UpdateUniform(Shader::Mat4, "u_ModelMat", modelMatrix);
    m_DrawData.UpdateUniform(Shader::Vec3, "u_Color", glm::vec3(1.0f));

    Renderer::Draw(m_DrawData);
}

void SpriteRenderer::Draw(Texture& texture)
{
    if (!m_HasTextureShader)
    {
        m_Shader = m_TextureShader;
        m_DrawData.UpdateShader(*m_Shader);
        m_HasTextureShader = true;
    }
    m_DrawData.UpdateTexture(texture);
    m_DrawData.UpdateUniform(Shader::Vec2, "u_Tile", texture.Tile);
    Draw();
}

glm::mat4 SpriteRenderer::GetModelMatrix()
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, Position);
    modelMat = glm::scale(modelMat, Scale);

    modelMat = glm::rotate(modelMat, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return modelMat;
}

