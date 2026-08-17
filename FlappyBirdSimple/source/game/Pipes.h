#pragma once

#include "PCH.h"
#include "SpriteRenderer.h"
#include "Physics.h"
#include <glm/gtc/matrix_transform.hpp>

struct PipeSets
{
	enum PipesType
	{
		Low = 0,
		Middle = 1,
		High = 2,
	};

	PipeSets(PipesType type)
	{
		PipesScaleAndPositon scaleAndPos;

		scaleAndPos.Pipe_1_Position.y = 660.0f;
		scaleAndPos.Pipe_2_Position.y = -100.0f;
		m_Pipes.insert({ Low,scaleAndPos });

		scaleAndPos.Pipe_1_Position.y = 820.0f;
		scaleAndPos.Pipe_2_Position.y = 60.0f;
		m_Pipes.insert({ Middle,scaleAndPos });

		scaleAndPos.Pipe_1_Position.y = 960.0f;
		scaleAndPos.Pipe_2_Position.y = 200.0f;
		m_Pipes.insert({ High,scaleAndPos });

		Pipe_1.Position = m_Pipes[type].Pipe_1_Position;
		Pipe_1.Scale = m_Pipes[type].Pipe_1_Scale;
		Pipe_1.Rotation = glm::vec3(180.0f, 0.0f, 0.0f);

		Pipe_2.Position = m_Pipes[type].Pipe_2_Position;
		Pipe_2.Scale = m_Pipes[type].Pipe_2_Scale;
		Pipe_2.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	~PipeSets() {}

	void Update() 
	{
		Pipe_1.Position.x = Position.x;
		Pipe_2.Position.x = Position.x;
	}

	void Draw(Texture& texture) 
	{
		Pipe_1.Draw(texture);
		Pipe_2.Draw(texture);
	}

	SpriteRenderer Pipe_1;
	SpriteRenderer Pipe_2;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
private:
	glm::mat4 GetModelMatrix() 
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);

		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		return translation * scale * rotation;
	}
private:
	struct PipesScaleAndPositon
	{
		glm::vec3 Pipe_1_Position = glm::vec3(0.0f), Pipe_1_Scale = glm::vec3(0.4f, 3.0f, 0.0f);
		glm::vec3 Pipe_2_Position = glm::vec3(0.0f), Pipe_2_Scale = glm::vec3(0.4f, 3.0f, 0.0f);
	};
	std::unordered_map<PipesType, PipesScaleAndPositon> m_Pipes = std::unordered_map<PipesType, PipesScaleAndPositon>();
};

class Pipes
{
public:
	Pipes(WindowConfig& windowConfig) : m_WindowConfig(windowConfig)
	{
		m_GreenPipeTexture = &ResourceManager::GetResource<Texture>("Pipe_Texture");

		m_Pipes.push_back(new PipeSets(PipeSets::Low));
		m_Pipes.push_back(new PipeSets(PipeSets::Middle));
		m_Pipes.push_back(new PipeSets(PipeSets::High));

		for (int pipeIndex = 0; pipeIndex < m_Pipes.size(); pipeIndex++)
		{
			PipeSets& pipeSet = *m_Pipes[pipeIndex];
			pipeSet.Position.x = (float)m_WindowConfig.Width + (c_PipeDistance * pipeIndex);
			pipeSet.Update();
		}

		PipeSets lastPipeSet = *m_Pipes[m_Pipes.size() - 1];
		m_InitialLastPosition = lastPipeSet.Position;
	}
	~Pipes() 
	{
		for (auto pipeSet : m_Pipes) delete pipeSet;
		m_Pipes.clear();
	}

	inline void Update(float deltaTime) 
	{
		//TODO : have to fix the pipe positioning
		for (auto pipeSet : m_Pipes)
		{
			pipeSet->Position.x -= deltaTime * m_PipesSpeed;
			if (pipeSet->Position.x < -50.0f)
			{
				PipeSets* lastPipeSet = nullptr;
				float lastXPosition = -1000000000000000000.0f;
				for (auto newPipeSet : m_Pipes)
				{
					if (newPipeSet->Position.x > lastXPosition) 
					{
						lastPipeSet = newPipeSet;
						lastXPosition = newPipeSet->Position.x;
					}
				}
				pipeSet->Position.x = lastPipeSet->Position.x + c_PipeDistance;
			}
			pipeSet->Update();
		}
	}

	inline void Draw() 
	{
		for (auto pipeSet : m_Pipes)
		{
			pipeSet->Draw(*m_GreenPipeTexture);
		}
	}

	inline bool CheckCollision(glm::vec3 Position, glm::vec3 Scale)
	{
		for (auto pipeSet : m_Pipes)
		{
			if(Physics::CheckCollision(Position, Scale * 200.0f, pipeSet->Pipe_1.Position, pipeSet->Pipe_1.Scale * 200.0f) ||
			   Physics::CheckCollision(Position, Scale * 200.0f, pipeSet->Pipe_2.Position, pipeSet->Pipe_2.Scale * 200.0f)
			  )
			{
				return true;
			}
		}
		return false;
	}
	inline void SetSpeed(float newSpeed) {m_PipesSpeed = newSpeed;}
	inline bool CheckBirdCrossing(glm::vec3 birdPosition, int& collidedPipeIndex) 
	{
		for (int i = 0; i < m_Pipes.size(); i++) 
		{
			PipeSets* pipeSet = m_Pipes[i];
			if (pipeSet->Position.x < birdPosition.x) 
			{
				collidedPipeIndex = i;
				return true;
			}
		}
		return false;
	}
private:
	const float c_PipeDistance = 400.0f;

	float m_PipesSpeed   = 100.0f;

	glm::vec3 m_InitialLastPosition = glm::vec3(0.0f);

	Texture* m_GreenPipeTexture = nullptr;
	std::vector<PipeSets*> m_Pipes;
	WindowConfig& m_WindowConfig;
};