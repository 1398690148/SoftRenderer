#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "LambertVertexShader.h"
#include "PointLightPS.h"
#include <tbb/tick_count.h>
#include <PixelConstantBuffer.h>
#include <Light.h>

glm::vec3 eye(0, 0, 5);

App::App()
	: wnd(666, 500, "The Donkey Fart Box"), camera(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))
{
	parser.parse("../src/Casli/Configure/PointLight.scene", wnd.Gfx(), false);
	camera = Camera(parser.m_scene.m_CameraPos, parser.m_scene.m_CameraFront, parser.m_scene.m_CameraUp);
	wnd.Gfx().SetProjection(glm::perspective(glm::radians(parser.m_scene.m_FrustumFovy), 4.0f / 3.0f, parser.m_scene.m_FrustumNear, parser.m_scene.m_FrustumFar));
	wnd.Gfx().SetVertexShader(new LambertVertexShader());
	wnd.Gfx().SetPixelShader(new PointPixelShader());
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
	wnd.Gfx().BeginFrame(1.f, 1.f, 1.f);
	auto &drawable = parser.m_scene.m_entities;
	auto &lights = parser.m_scene.m_lights;

	for (auto light : lights)
	{
		light->rotate(1.0f, glm::vec3(1, 0, 0));
		light->Bind(wnd.Gfx());
	}
	glm::vec3 viewPos = camera.GetPosition();
	PixelConstantBuffer *p = new PixelConstantBuffer(wnd.Gfx(), (unsigned char *)&viewPos, 108, sizeof(glm::vec3));
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
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}
	}
	while (const auto delta = wnd.mouse.Read())
	{
		switch (delta->GetType())
		{
		case Mouse::Event::Type::Move:
			camera.Rotate((float)delta->GetPosX(), (float)delta->GetPosY());
			break;
		case Mouse::Event::Type::WheelUp:
			camera.Translate((float)-delta->GetPosX());
		case Mouse::Event::Type::WheelDown:
			camera.Translate((float)delta->GetPosX());
			break;
		}
	}
	wnd.Gfx().EndFrame();
	int fps = 1.0f / (tbb::tick_count::now() - t0).seconds();
	wnd.SetTitle("FPS: " + std::to_string(fps));
}