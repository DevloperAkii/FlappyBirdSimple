#pragma once
#include <imgui/imgui.h>

namespace ImGuiLayer
{
	void Init();

	void StartFrame();
	void EndFrame();

	void Shutdown();
}