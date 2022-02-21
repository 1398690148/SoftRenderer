#pragma once
#include "./Platform/Window.h"
#include "Casli/Scene/Camera.h"
#include "Casli/Scene/Model.h"
#include "Casli/Test/Box.h"
#include "Casli/Test/Plane.h"
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
	Model model{ wnd.Gfx(), "../src/Casli/Model/nanosuit/nanosuit.obj" };
	//Box box{ wnd.Gfx() };
	//Plane plane{ wnd.Gfx() };
};