#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 eye(1, 0, 3);

App::App()
	:
	wnd(800, 600, "The Donkey Fart Box"), camera(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45)
{
	glm::mat4x4 Projection = glm::mat4x4(1.0);//glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
	Projection[3][2] = -1.0 / eye.length();
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
	wnd.Gfx().ClearBuffer(0.5f, 0.5f, 0.5f);
	wnd.Gfx().SetCamera(camera.GetMatrix());
	wnd.Gfx().Draw();
	wnd.Gfx().EndFrame();
}