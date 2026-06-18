#include "Texture.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

Texture::Texture(std::string textureFilePath)
{
    if (!s_Init) 
    {
        stbi_set_flip_vertically_on_load(true);
        s_Init = true;
    }

    if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
    {
        glGenTextures(1, &m_TextureID);

        unsigned char* data = stbi_load(textureFilePath.c_str(), &m_Width, &m_Height, &m_NrComponent, 0);
        if (data)
        {
            GLenum format = 0;
            if (m_NrComponent == 1)
                format = GL_RED;
            else if (m_NrComponent == 3)
                format = GL_RGB;
            else if (m_NrComponent == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, m_TextureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        }
        if(data) stbi_image_free(data);
        ASSERT(!data, "Texture failed to load at path:", textureFilePath)
        m_TextureSlot = s_TexturesCount;
        s_TexturesCount++;
    }
}

Texture::~Texture()
{
    if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
    {
        glDeleteTextures(1, &m_TextureID);
    }
}

void Texture::Bind()
{
    if (m_Bounded) return;
    if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
    {
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }
    m_Bounded = true;
}

void Texture::UnBind()
{
    m_Bounded = false;
    if (Renderer::s_CurrentAPI == Renderer::OPENGL)
    {
        glBindTexture(GL_TEXTURE_2D ,0);
    }
}
