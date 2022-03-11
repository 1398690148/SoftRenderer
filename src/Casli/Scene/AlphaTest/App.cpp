#include "App.h"
#include <glm/gtc/matrix_transform.hpp>
#include "AlphaTestVS.h"
#include "AlphaTestPS.h"
#include <iostream>
#include <tbb/tick_count.h>

glm::vec3 eye(0, 0, 5);

App::App()
	: wnd(800, 600, "The Donkey Fart Box") 
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
	InitMatrix();
	wnd.Gfx().BeginFrame(1.f, 1.f, 1.f);
	auto &drawable = parser.m_scene.m_entities;
	for (auto iter : drawable)
	{
		CBuffer.clear();
		glm::mat4 Model = iter.second->GetModelMatrix();
		CBuffer.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * Model);
		CBuffer.push_back(Model);
		CBuffer.push_back(glm::transpose(Model));
		iter.second->Bind((unsigned char *)&CBuffer[0], sizeof(glm::mat4) * CBuffer.size());
		iter.second->Draw();
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
	//std::cout << "Time: " << (tbb::tick_count::now() - t0).seconds() << std::endl;
	int fps = 1.0f / (tbb::tick_count::now() - t0).seconds();
	wnd.SetTitle("FPS: " + std::to_string(fps));
}

void App::InitMatrix()
{

}
