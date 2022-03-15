#include "App.h"
#include <glm/gtc/matrix_transform.hpp>
#include <tbb/tick_count.h>
#include <PixelConstantBuffer.h>
#include <Light.h>
#include "AlphaTestVS.h"
#include "AlphaTestPS.h"
#include <iostream>

App::App()
	: wnd(666, 500, "The Donkey Fart Box") 
{
	parser.parse("../src/Casli/Configure/AlphaTest.scene", wnd.Gfx(), false);
	camera = Camera(parser.m_scene.m_CameraPos, parser.m_scene.m_CameraFront, parser.m_scene.m_CameraUp);
	wnd.Gfx().SetProjection(glm::perspective(glm::radians(parser.m_scene.m_FrustumFovy), 4.0f / 3.0f, parser.m_scene.m_FrustumNear, parser.m_scene.m_FrustumFar));
	wnd.Gfx().SetVertexShader(new AlphaTestVS());
	wnd.Gfx().SetPixelShader(new AlphaTestPS());
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
	auto &drawable = parser.m_scene.m_entities;
	auto &lights = parser.m_scene.m_lights;

	for (auto light : lights)
	{
		light->Bind(wnd.Gfx());
	}
	glm::vec3 viewPos = camera.GetPosition();
	PixelConstantBuffer *p = new PixelConstantBuffer(wnd.Gfx(), (unsigned char *)&viewPos, 36, sizeof(glm::vec3));
	p->Bind(wnd.Gfx());

	for (auto iter : drawable)
	{
		CBuffer.clear();
		glm::mat4 Model = iter->GetModelMatrix();
		CBuffer.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * Model);
		CBuffer.push_back(Model);
		CBuffer.push_back(glm::transpose(Model));
		iter->Bind((unsigned char *)&CBuffer[0], sizeof(glm::mat4) * CBuffer.size());
		iter->Draw();
	}
	CBuffer.clear();
	glm::mat4 Model = glm::mat4(1.0);
	CBuffer.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * Model);
	CBuffer.push_back(Model);
	CBuffer.push_back(glm::transpose(Model));
	plane.Bind((unsigned char *)&CBuffer[0], sizeof(glm::mat4) * CBuffer.size());
	plane.Draw();
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
			camera.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			camera.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			camera.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			camera.Translate({ dt,0.0f,0.0f });
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

