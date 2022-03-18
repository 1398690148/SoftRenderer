#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <SampleTextureVS.h>
#include <SampleTexturePS.h>
#include <tbb/tick_count.h>

glm::vec3 eye(0, 0, -2);

App::App()
	: wnd(666, 500, "The Donkey Fart Box")
{
	parser.parse("../src/Casli/Configure/MipMap.scene", wnd.Gfx(), false);
	camera = Camera(parser.m_scene.m_CameraPos, parser.m_scene.m_CameraFront, parser.m_scene.m_CameraUp);
	wnd.Gfx().SetProjection(glm::perspective(glm::radians(parser.m_scene.m_FrustumFovy), 4.0f / 3.0f, parser.m_scene.m_FrustumNear, parser.m_scene.m_FrustumFar));

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
	tbb::tick_count t0 = tbb::tick_count().now();
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().SetCamera(camera.GetMatrix());
	wnd.Gfx().BeginFrame(0.5f, 0.5f, 0.5f);
	auto &drawable = parser.m_scene.m_Entities;
	for (auto iter : drawable)
	{
		iter->Draw(wnd.Gfx());
	}
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}
		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		}
	}
	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			camera.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			camera.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			camera.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			camera.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			camera.Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			camera.Translate({ 0.0f,-dt,0.0f });
		}
	}
	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			camera.Rotate((float)delta->x, (float)delta->y);
		}
	}
	wnd.Gfx().EndFrame();
	int fps = 1.0f / (tbb::tick_count::now() - t0).seconds();
	wnd.SetTitle("FPS: " + std::to_string(fps));
}
