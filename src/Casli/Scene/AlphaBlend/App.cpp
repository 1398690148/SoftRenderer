#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "AlphaBlendVS.h"
#include "AlphaBlendPS.h"
#include "SampleTextureVS.h"
#include "SampleTexturePS.h"
#include <iostream>
#include <time.h>
#include <tbb/tick_count.h>

glm::vec3 eye(0, 0, 5);

App::App()
	: wnd(800, 600, "The Donkey Fart Box"), camera(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))
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
	tbb::tick_count t0 = tbb::tick_count().now();
	wnd.Gfx().SetCamera(camera.GetMatrix());
	wnd.Gfx().SetVertexShader(new SampleTextureVS());
	wnd.Gfx().SetPixelShader(new SampleTexturePS());
	InitMatrix();
	wnd.Gfx().BeginFrame(1.f, 1.f, 1.f);
	model.Bind((unsigned char *)(&CBuffer[1]), sizeof(glm::mat4) * 3);
	model.Draw();
	wnd.Gfx().SetVertexShader(new AlphaBlendVS());
	wnd.Gfx().SetPixelShader(new AlphaBlendPS());
	plane.Bind((unsigned char *)(&CBuffer[0]), sizeof(glm::mat4));
	plane.Draw();
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
	CBuffer.clear();
	glm::mat4 Model = glm::mat4(1.0);
	Model = glm::scale(Model, glm::vec3(2, 2, 1));
	CBuffer.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * Model);

	glm::mat4 MVP = glm::mat4(1.0);
	glm::mat4 MT = glm::mat4(1.0);
	glm::mat4 M = glm::mat4(1.0);
	MVP = glm::translate(MVP, glm::vec3(0, -12.f, -7));
	M = glm::translate(M, glm::vec3(0, -12.f, -7));
	MT = glm::translate(MT, glm::vec3(0, -12.f, -7));
	MT = glm::transpose(glm::inverse(MT));
	MVP = wnd.Gfx().GetProjection() * camera.GetMatrix() * MVP;
	CBuffer.push_back(MVP);
	CBuffer.push_back(M);
	CBuffer.push_back(MT);
}