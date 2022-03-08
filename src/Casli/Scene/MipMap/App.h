#pragma once
#include "Platform/Window.h"
#include <Camera.h>
#include <Model.h>
#include <Box.h>
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
	Plane plane{ wnd.Gfx(), "../src/Casli/Image/wall_diffuse.jpg", 1 };
	Plane mipPlane{ wnd.Gfx(), "../src/Casli/Image/wall_diffuse.jpg", 8 };
	std::vector<std::vector<glm::mat4>> planesCBuffer;
};