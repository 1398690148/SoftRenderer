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
	Plane plane{ wnd.Gfx(), "../src/Casli/Image/testbilinear.png", 1 };
	Plane billinearPlane{ wnd.Gfx(), "../src/Casli/Image/testbilinear.png", 1, FILTER_LINEAR_MIP_POINT };
	std::vector<std::vector<glm::mat4>> planesCBuffer;
};