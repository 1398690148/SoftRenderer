#pragma once
#include "Platform/Window.h"
#include <Camera.h>
#include <AlphaPlane.h>
#include <Model.h>
#include <Box.h>
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
	AlphaPlane plane{ wnd.Gfx(), "", 1 };
	Model model{ wnd.Gfx(), "../src/Casli/Model/nanosuit/nanosuit.obj" };
	std::vector<glm::mat4 > CBuffer;
};