#include "Timer.h"

Time::Time()
{
	m_LastFrameTime = std::chrono::high_resolution_clock::now();
}

Time::~Time()
{
}

void Time::Tick()
{
	auto currentFrameTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = currentFrameTime - m_LastFrameTime;
	m_LastFrameTime = currentFrameTime;

	DeltaTime = elapsed.count();
	DeltaTime *= TimeScale;
}
