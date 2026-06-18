#pragma once
#include <string>
#include <variant>
#include <glm/glm.hpp>
#include <unordered_map>

class Shader
{
public:
	enum UniformType
	{
		None = -1,
		Int,
		Float,
		Vec2,
		Vec3,
		Mat4,
	};

	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
	~Shader();
	void Bind();
	void UnBind();
	void SetUniform(UniformType uniformType, std::string name, std::variant<int, float, glm::vec2, glm::vec3, glm::mat4> value);

private:
	uint32_t CompileOpenGLShader(const unsigned int type, const std::string& file);
	uint32_t CreateOpenGLShader(const std::string VertexShaderFile, const std::string FragmentShaderFile);
	uint32_t GetUnifromLocation(const std::string& name);
private:
	uint32_t m_ShaderID = 0;
	bool m_Bounded = false;
	std::unordered_map<std::string, uint32_t> m_UnfiromLocationCache;
};