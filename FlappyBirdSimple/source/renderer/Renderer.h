#pragma once

#include "PCH.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

struct DrawData 
{
public:
	DrawData(Shader* shader);
	DrawData(Shader* shader, Texture* texture);
	DrawData();
	~DrawData();

	void GenerateDrawData(std::vector<float>& vertices, std::vector<uint32_t>& indices);
	void UpdateShader(Shader* shader);
	void UpdateTexture(Texture* texture);
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
private:
	friend class Renderer;

	Shader* m_Shader = nullptr;
	Texture* m_Texture = nullptr;

	uint32_t m_VAO = 0;
	uint32_t m_VBO = 0;
	uint32_t m_EBO = 0;
	uint32_t m_IndicesCount = 0;


	bool m_Drawable = false;
	bool m_ExistInDrawCall = false;
	bool m_HasTexture = false;
};

class Renderer 
{
public:
	enum RendererAPI 
	{
		NONE = -1,
		OPENGL = 0
	};

	static void Init(RendererAPI api);
	static void SetClearColor(float r, float g, float b, float a);
	static void StartFrame();
	static void Draw(DrawData& drawData);
	static void EndFrame();
private:
	friend struct DrawData;
	friend class Shader;
	friend class Texture;

	inline static float s_ClearColorRed = 0.0f, s_ClearColorGreen= 0.0f, s_ClearColorBlue = 0.0f, s_ClearColorAlpha = 0.0f;
	inline static int s_FrameBufferWidth = 0, s_FrameBufferHeight = 0;
	inline static int s_TextureSlot = 0;
	inline static std::vector<DrawData*> s_DataToDraw = std::vector<DrawData*>();
	inline static RendererAPI s_CurrentAPI = RendererAPI::NONE;
};