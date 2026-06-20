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
        glGenTextures(1, &m_TextureHandel);

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

            glBindTexture(GL_TEXTURE_2D, m_TextureHandel);
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        }
        if(data) stbi_image_free(data);
        ASSERT(!data, "Texture failed to load at path:", textureFilePath)
    }
}

Texture::~Texture()
{
    if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
    {
        glDeleteTextures(1, &m_TextureHandel);
    }
}

void Texture::Bind()
{
    if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
    {
        glBindTexture(GL_TEXTURE_2D, m_TextureHandel);
    }
}

void Texture::UnBind()
{
    if (Renderer::s_CurrentAPI == Renderer::OPENGL)
    {
        glBindTexture(GL_TEXTURE_2D ,0);
    }
}