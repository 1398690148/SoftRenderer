#pragma once
#include "Platform/Window.h"
#include <Camera.h>
#include <Model.h>
#include <Plane.h>
#include <DirectionalLight.h>
#include <PointLight.h>
#include <SpotLight.h>

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
	DirectionalLight directionalLight{ wnd.Gfx(), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1, 0, 1), 0 };
	PointLight pointLight{ wnd.Gfx(), glm::vec3(1.f, 0.f, 0.f), glm::vec3(-2, 2, -6.f), 0.2f, 0.4f, 0.2f, 24 };
	SpotLight spotLight{ wnd.Gfx(), glm::vec3(0, 1, 0), glm::vec3(0, 1, -4), glm::vec3(0, 0, -1), 0.2f, 0.4f, 0.2f, std::cos(acos(-1) / 180 * 10), 60 };
	Model model{ wnd.Gfx(), "../src/Casli/Model/nanosuit/nanosuit.obj" };
	//Plane plane{ wnd.Gfx(), 1 };
};