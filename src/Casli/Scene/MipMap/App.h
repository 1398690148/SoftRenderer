#pragma once
#include "Platform/Window.h"
#include "Platform/CasliTimer.h"
#include <Camera.h>
#include <Plane.h>
#include <SceneParse.h>

class App
{
public:
	App();
	// master frame / message loop
	int Go();
private:
	void DoFrame();

	void InitMatrix();
private:
	Window wnd;
	Camera camera;
	CasliTimer timer;
	float speed_factor = 1.0f;
	SceneParse parser;
};