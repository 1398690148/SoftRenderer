#pragma once
#include "Platform/Window.h"
#include <Camera.h>
#include <SceneParse.h>
#include <PointLight.h>
#include "Platform/CasliTimer.h"

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
};