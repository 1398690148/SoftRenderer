#pragma once
#include "Platform/Window.h"
#include "Platform/CasliTimer.h"
#include <Camera.h>
#include <Plane.h>
#include <Model.h>

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
	Plane plane{ wnd.Gfx(), "../src/Casli/Image/wall_diffuse.jpg", 1 };
	Plane mipPlane{ wnd.Gfx(), "../src/Casli/Image/wall_diffuse.jpg", 10 };
	Model lamp{ wnd.Gfx(), "../src/Casli/Model/lamp/lamp.obj" };
	Model lamp1{ wnd.Gfx(), "../src/Casli/Model/lamp/lamp_green.obj" };
	Model lamp2{ wnd.Gfx(), "../src/Casli/Model/lamp/lamp_red.obj" };
	//Model lawn{ wnd.Gfx(), "../src/Casli/Model/lawn/lawn.obj" };
	std::vector<std::vector<glm::mat4>> planesCBuffer;
	float z = 0;
};