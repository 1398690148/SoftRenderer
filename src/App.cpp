#include "App.h"
#include <math.h>

App::App()
	:
	wnd(800, 600, "The Donkey Fart Box"), camera(Vec3f(0, 0, 3), Vec3f(0, 0, 0), Vec3f(0, 1, 0), 45)
{
	Matrix Projection = Matrix::identity();
	Projection[3][2] = -1.0 / (Vec3f(0, 0, 3)).norm();
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