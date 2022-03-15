#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <SampleTextureVS.h>
#include <SampleTexturePS.h>
#include <tbb/tick_count.h>

glm::vec3 eye(0, 0, 3);

App::App()
	: wnd(666, 500, "The Donkey Fart Box"), camera(eye, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0))
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.f), 4.0f / 3.0f, 0.1f, 100.f);
	wnd.Gfx().SetProjection(Projection);

	wnd.Gfx().SetVertexShader(new SampleTextureVS());
	wnd.Gfx().SetPixelShader(new SampleTexturePS());
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
	wnd.Gfx().BeginFrame(0.5f, 0.5f, 0.5f);
	plane.Bind((unsigned char *)(&planesCBuffer[0][0]), sizeof(glm::mat4));
	plane.Draw();
	mipPlane.Bind((unsigned char *)(&planesCBuffer[1][0]), sizeof(glm::mat4));
	mipPlane.Draw();
	//lamp.Bind((unsigned char *)(&planesCBuffer[2][0]), sizeof(glm::mat4));
	//lamp.Draw();
	//lamp1.Bind((unsigned char *)(&planesCBuffer[3][0]), sizeof(glm::mat4));
	//lamp1.Draw();
	//lamp2.Bind((unsigned char *)(&planesCBuffer[4][0]), sizeof(glm::mat4));
	//lamp2.Draw();
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
	//std::cout << "Time: " << (tbb::tick_count::now() - t0).seconds() << std::endl;
	int fps = 1.0f / (tbb::tick_count::now() - t0).seconds();
	wnd.SetTitle("FPS: " + std::to_string(fps));
}

void App::InitMatrix()
{
	planesCBuffer.clear();
	std::vector<glm::mat4> v;
	glm::mat4 Model = glm::mat4(1.0);
	Model = glm::scale(Model, glm::vec3(0.5, 2, 1));
	Model = glm::rotate(Model, glm::radians(-80.0f), glm::vec3(1, 0, 0));
	Model = glm::translate(Model, glm::vec3(0.51, 0, 0));

	v.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * Model);
	v.push_back(Model);
	v.push_back(glm::transpose(glm::inverse(Model)));
	planesCBuffer.push_back(v);
	Model = glm::translate(Model, glm::vec3(-1.01, 0, 0));
	v[0] = wnd.Gfx().GetProjection() * camera.GetMatrix() * Model;
	planesCBuffer.push_back(v);

	glm::mat4 lampModel = glm::mat4(1.0);
	lampModel = glm::scale(lampModel, glm::vec3(0.04, 0.04, 0.04));
	lampModel = glm::rotate(lampModel, glm::radians(30.f), glm::vec3(1, 0, 0));
	v.clear();
	v.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * lampModel);
	v.push_back(lampModel);
	v.push_back(glm::transpose(glm::inverse(lampModel)));
	planesCBuffer.push_back(v);

	glm::mat4 lamp1Model = glm::mat4(1.0);
	lamp1Model = glm::scale(lamp1Model, glm::vec3(0.04, 0.04, 0.04));
	lamp1Model = glm::rotate(lamp1Model, glm::radians(30.f), glm::vec3(1, 0, 0));
	lamp1Model = glm::translate(lamp1Model, glm::vec3(-5, 0, 0));
	v.clear();
	v.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * lamp1Model);
	v.push_back(lamp1Model);
	v.push_back(glm::transpose(glm::inverse(lamp1Model)));
	planesCBuffer.push_back(v);

	glm::mat4 lamp2Model = glm::mat4(1.0);
	lamp2Model = glm::scale(lamp2Model, glm::vec3(0.04, 0.04, 0.04));
	lamp2Model = glm::rotate(lamp2Model, glm::radians(30.f), glm::vec3(1, 0, 0));
	lamp2Model = glm::translate(lamp2Model, glm::vec3(5, 0, 0));
	v.clear();
	v.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * lamp2Model);
	v.push_back(lamp2Model);
	v.push_back(glm::transpose(glm::inverse(lamp2Model)));
	planesCBuffer.push_back(v);

	//glm::mat4 lawnModel = glm::mat4(1.0);
	//lawnModel = glm::scale(lawnModel, glm::vec3(1, 1, 1));
	////lamp2Model = glm::rotate(lamp2Model, glm::radians(30.f), glm::vec3(1, 0, 0));
	////lamp2Model = glm::translate(lamp2Model, glm::vec3(5, 0, 0));
	//v.clear();
	//v.push_back(wnd.Gfx().GetProjection() * camera.GetMatrix() * lawnModel);
	//v.push_back(lawnModel);
	//v.push_back(glm::transpose(glm::inverse(lawnModel)));
	//planesCBuffer.push_back(v);
}
