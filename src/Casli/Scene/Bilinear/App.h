#pragma once
#include "Platform/Window.h"
#include <Camera.h>
#include <Model.h>
#include <Plane.h>
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
	Plane plane{ wnd.Gfx(), "../src/Casli/Image/testbilinear.png", 1 };
	Plane billinearPlane{ wnd.Gfx(), "../src/Casli/Image/testbilinear.png", 1, FILTER_LINEAR_MIP_POINT };
	std::vector<std::vector<glm::mat4>> planesCBuffer;
};