#pragma once
#include "Platform/Window.h"
#include "Platform/CasliTimer.h"
#include <Camera.h>
#include <AlphaPlane.h>
#include <Model.h>
#include <SceneParse.h>

class App
{
public:
	App();
	// master frame / message loop
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
	Camera camera;
	CasliTimer timer;
	float speed_factor = 1.0f;
	SceneParse parser;
	std::vector<glm::mat4> CBuffer;
};