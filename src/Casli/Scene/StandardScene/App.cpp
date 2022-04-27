#include "App.h"
#include <glm/gtc/matrix_transform.hpp>
#include <tbb/tick_count.h>
#include <Model.h>

App::App()
	: wnd(666, 500, "FPS:")
{
	parser.parse("../src/Casli/Configure/StandardScene.scene", wnd.Gfx());
	camera = Camera(parser.m_scene.m_CameraPos, parser.m_scene.m_CameraFront, parser.m_scene.m_CameraUp);
	wnd.Gfx().SetProjection(glm::perspective(glm::radians(parser.m_scene.m_FrustumFovy), 4.0f / 3.0f, parser.m_scene.m_FrustumNear, parser.m_scene.m_FrustumFar));
}

App::App(const char * path) 
	: wnd(666, 500, "FPS:")
{
	parser.parse(path, wnd.Gfx());
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
	wnd.Gfx().BeginFrame(0, 0, 0);
	glm::mat4 rotate = glm::mat4(1.0);
	rotate = glm::rotate(rotate, glm::radians(3.f), glm::vec3(0, 1, 0));
	auto &models = parser.m_scene.m_Models;
	auto &drawable = parser.m_scene.m_Entities;
	auto &lights = parser.m_scene.m_Lights;
	for (auto iter : models)
	{
		if (iter.second)
		{
			for (auto light : lights)
			{
				light->Bind(wnd.Gfx(), rotate);
			}
		}
		iter.first->Draw(wnd.Gfx(), glm::mat4(1.0));
	}
	for (auto iter : drawable)
	{
		if (iter.second)
		{
			for (auto light : lights)
			{
				light->Bind(wnd.Gfx(), glm::mat4(1.0));
			}
		}
		iter.first->Draw(wnd.Gfx());
	}
	for (auto light : lights)
	{
		light->Draw(wnd.Gfx());
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
			camera.Translate({ 0.0f, 0.0f, dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			camera.Translate({ dt, 0.0f, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			camera.Translate({ 0.0f, 0.0f, -dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			camera.Translate({ -dt, 0.0f, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			camera.Translate({ 0.0f, dt, 0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			camera.Translate({ 0.0f, -dt, 0.0f });
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