#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 eye(0, 0, 5);

App::App()
	: wnd(800, 600, "The Donkey Fart Box"), camera(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45)
{
	glm::mat4 Projection = glm::perspective(glm::radians(30.f), 4.0f / 3.0f, 0.1f, 100.f);
	wnd.Gfx().SetProjection(Projection);
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
	directionalLight.Bind(wnd.Gfx());
	pointLight.Bind(wnd.Gfx());
	spotLight.Bind(wnd.Gfx());
	model.Draw(wnd.Gfx());
	//plane.Draw(wnd.Gfx());
	wnd.Gfx().EndFrame();
}