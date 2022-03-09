#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <SampleTextureVS.h>
#include <SampleTexturePS.h>

glm::vec3 eye(0, 0, 3);

App::App()
	: wnd(800, 600, "The Donkey Fart Box"), camera(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45)
{
	glm::mat4 Projection = glm::perspective(glm::radians(30.f), 4.0f / 3.0f, 0.1f, 100.f);
	wnd.Gfx().SetProjection(Projection);

	wnd.Gfx().SetVertexShader(new SampleTextureVS());
	wnd.Gfx().SetPixelShader(new SampleTexturePS());

	std::vector<glm::mat4> v;
	glm::mat4 Model = glm::mat4(1.0);
	Model = glm::translate(Model,  glm::vec3(0.51, 0, 0));

	v.push_back(Projection * camera.GetMatrix() * Model);
	v.push_back(Model);
	v.push_back(glm::transpose(glm::inverse(Model)));
	planesCBuffer.push_back(v);
	Model = glm::translate(Model, glm::vec3(-1.01, 0, 0));
	v[0] = Projection * camera.GetMatrix() * Model;
	planesCBuffer.push_back(v);
}

int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	wnd.Gfx().SetCamera(camera.GetMatrix());
	wnd.Gfx().BeginFrame(1.f, 1.f, 1.f);
	plane.Bind(wnd.Gfx(), (unsigned char *)(&planesCBuffer[0][0]), sizeof(glm::mat4));
	plane.Draw(wnd.Gfx());
	billinearPlane.Bind(wnd.Gfx(), (unsigned char *)(&planesCBuffer[1][0]), sizeof(glm::mat4));
	billinearPlane.Draw(wnd.Gfx());
	wnd.Gfx().EndFrame();
}