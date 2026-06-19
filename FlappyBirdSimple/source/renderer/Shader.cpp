#include "Shader.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
	{
		m_ShaderID = CreateOpenGLShader(vertexShaderPath, fragmentShaderPath);
	}
}

Shader::~Shader()
{
	if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
	{
		glDeleteProgram(m_ShaderID);
	}
}

void Shader::Bind()
{
	if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
	{
		if (m_Bounded) return;
		glUseProgram(m_ShaderID);
	}
	m_Bounded = true;
}

void Shader::UnBind()
{
	if (Renderer::s_CurrentAPI == Renderer::OPENGL) 
	{
		glUseProgram(0);
	}
	m_Bounded = false;
}

void Shader::SetUniform(UniformType uniformType, std::string name, std::variant<int, float, glm::vec2, glm::vec3, glm::mat4> value)
{
	Bind();
	if (Renderer::s_CurrentAPI == Renderer::OPENGL)
	{
		switch (uniformType)
		{
		case Shader::None:
			break;
		case Shader::Int:
			if (std::holds_alternative<int>(value))
			{
				int i = std::get<int>(value);
				glUniform1i(GetUnifromLocation(name), i);
			}
			break;
		case Shader::Float:
			if (std::holds_alternative<float>(value)) 
			{
				float f = std::get<float>(value);
				glUniform1f(GetUnifromLocation(name), f);
			}
			break;
		case Shader::Vec2:
			if (std::holds_alternative<glm::vec2>(value))
			{
				glm::vec2 vec2 = std::get<glm::vec2>(value);
				glUniform2f(GetUnifromLocation(name), vec2.x, vec2.y);
			}
			break;
		case Shader::Vec3:
			if (std::holds_alternative<glm::vec3>(value))
			{
				glm::vec3 vec3 = std::get<glm::vec3>(value);
				glUniform3f(GetUnifromLocation(name), vec3.x, vec3.y, vec3.z);
			}
			break;
		case Shader::Mat4:
			if (std::holds_alternative<glm::mat4>(value))
			{
				glUniformMatrix4fv(GetUnifromLocation(name), 1, GL_FALSE, glm::value_ptr(std::get<glm::mat4>(value)));
			}
			break;
		}
	}
}

uint32_t Shader::CompileOpenGLShader(const unsigned int type, const std::string& file)
{
	std::ifstream stream(file);

	std::string line;
	std::string source;

	while (getline(stream, line)) {
		source += line + "\n";
	}

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)(alloca(lenght * sizeof(char)));
		(glGetShaderInfoLog(id, lenght, &lenght, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

uint32_t Shader::CreateOpenGLShader(const std::string VertexShaderFile, const std::string FragmentShaderFile)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileOpenGLShader(GL_VERTEX_SHADER, VertexShaderFile);
	unsigned int fs = CompileOpenGLShader(GL_FRAGMENT_SHADER, FragmentShaderFile);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

uint32_t Shader::GetUnifromLocation(const std::string& name) {

	if (m_UnfiromLocationCache.find(name) != m_UnfiromLocationCache.end()) {
		return m_UnfiromLocationCache[name];
	}

	int location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1) {
		std::cout << "[WARNING]" << name.c_str() << " dose not exist!" << std::endl;
	}
	m_UnfiromLocationCache[name] = location;
	return location;
}
