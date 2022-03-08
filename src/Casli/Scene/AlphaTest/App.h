#pragma once
#include "Platform/Window.h"
#include <Camera.h>
#include <AlphaPlane.h>
#include <Model.h>

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
	AlphaPlane plane{ wnd.Gfx(), "../src/Casli/Image/vase_plant.png", 1 };
	Model model{ wnd.Gfx(), "../src/Casli/Model/nanosuit/nanosuit.obj" };
	std::vector<glm::mat4 > planeCBuffer;
};