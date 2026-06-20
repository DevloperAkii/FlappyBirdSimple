#pragma once

#include "PCH.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <flat_map>

struct DrawData
{
public:
	void GenerateDrawData(std::vector<float>& vertices, std::vector<uint32_t>& indices);
	void UpdateShader(Shader& shader);
	void UpdateTexture(Texture& texture);
private:
	friend class Renderer;

	uint32_t m_ShaderID = 0;
	uint32_t m_TextureID = 0;

	uint32_t m_VAO = 0;
	uint32_t m_VBO = 0;
	uint32_t m_EBO = 0;
	uint32_t m_IndicesCount = 0;

	bool m_Drawable = false;
	bool m_HasTexture = false;
};

struct LineData 
{
	uint32_t m_VAO = 0;
	uint32_t m_VBO = 0;

	Shader m_Shader = Shader(RESOURCE_PATH"Assets/shaders/ColorVertexShader.glsl", RESOURCE_PATH"Assets/shaders/ColorFragmentShader.glsl");

	bool Generated = false;
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

	static void ResizeFrameBuffer(int width, int height);
	static void GetFrameBuffer(int* width,int* height);
	static int GetTextureSlot();

	static void SetClearColor(float r, float g, float b, float a);
	static void StartFrame();
	static void Draw(DrawData& drawData);
	static void DrawLine(glm::vec3 startPos, glm::vec3 endPos, glm::vec3 Color = glm::vec3(1.0f));
	static void EndFrame();

	static void SwapFrameBuffer();
private:
	friend struct DrawData;
	friend class Shader;
	friend class Texture;

	inline static float s_ClearColorRed = 0.0f, s_ClearColorGreen= 0.0f, s_ClearColorBlue = 0.0f, s_ClearColorAlpha = 0.0f;
	inline static int s_FrameBufferWidth = 0, s_FrameBufferHeight = 0;
	inline static int s_TextureSlot = 0;

	inline static std::flat_map<uint32_t, std::vector<DrawData>> s_DataToDraw = std::flat_map<uint32_t, std::vector<DrawData>>();
	inline static RendererAPI s_CurrentAPI = RendererAPI::NONE;
	inline static LineData s_LineData = LineData();
};