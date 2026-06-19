#pragma once
#include <chrono>

class Time 
{
public:
	Time();
	~Time();
	
	void Tick();
public:
	inline static double DeltaTime = 0.0f;
	inline static float TimeScale = 1.0f;
private:
	std::chrono::steady_clock::time_point m_LastFrameTime;
};