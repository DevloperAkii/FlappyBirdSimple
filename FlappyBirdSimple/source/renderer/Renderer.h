#pragma once

#include "PCH.h"
#include "Window.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

struct DrawData 
{
public:
	DrawData(Shader& shader);
	~DrawData();

	void GenerateDrawData(std::vector<float>& vertices, std::vector<uint32_t>& indices);
private:
	friend class Renderer;

	Shader& m_Shader;

	uint32_t m_VAO = 0;
	uint32_t m_VBO = 0;
	uint32_t m_EBO = 0;
	uint32_t m_IndicesCount = 0;

	bool m_Drawable = false;
	bool m_ExistInDrawCall = false;
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

	inline static float s_ClearColorRed = 0.0f, s_ClearColorGreen= 0.0f, s_ClearColorBlue = 0.0f, s_ClearColorAlpha = 0.0f;
	inline static int s_FrameBufferWidth = 0, s_FrameBufferHeight = 0;
	inline static glm::mat4 s_ProjectionMatrix = glm::mat4(1.0f);
	inline static RendererAPI s_CurrentAPI = RendererAPI::NONE;
	inline static std::vector<DrawData*> s_DataToDraw = std::vector<DrawData*>();
};