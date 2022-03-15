#pragma once
#include "Platform/Window.h"
#include <Camera.h>
#include <AlphaPlane.h>
#include <Model.h>
#include "Platform/CasliTimer.h"

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
	AlphaPlane plane{ wnd.Gfx(), "", 1 };
	Model model{ wnd.Gfx(), "../src/Casli/Model/diablo3_pose/diablo3_pose.obj" };
	std::vector<glm::mat4 > CBuffer;
};