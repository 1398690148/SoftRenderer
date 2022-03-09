#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "AlphaBlendVS.h"
#include "AlphaBlendPS.h"
#include "SampleTextureVS.h"
#include "SampleTexturePS.h"
#include <iostream>
#include <time.h>

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

//打印当前时间
std::string timetoStr() {
	char tmp[64];
	time_t t = time(NULL);
	tm *_tm = localtime(&t);
	int mm = _tm->tm_min;
	int ss = _tm->tm_sec;
	sprintf(tmp, "%02d  %02d", mm, ss);
	return std::string(tmp);
}

void App::DoFrame()
{
	wnd.Gfx().SetCamera(camera.GetMatrix());
	wnd.Gfx().SetVertexShader(new SampleTextureVS());
	wnd.Gfx().SetPixelShader(new SampleTexturePS());
	InitMatrix();
	wnd.Gfx().BeginFrame(1.f, 1.f, 1.f);
	std::cout << timetoStr() << std::endl;
	model.Bind(wnd.Gfx(), (unsigned char *)(&CBuffer[1]), sizeof(glm::mat4) * 3);
	model.Draw(wnd.Gfx());
	wnd.Gfx().SetVertexShader(new AlphaBlendVS());
	wnd.Gfx().SetPixelShader(new AlphaBlendPS());
	plane.Bind(wnd.Gfx(), (unsigned char *)(&CBuffer[0]), sizeof(glm::mat4));
	plane.Draw(wnd.Gfx());
	wnd.Gfx().EndFrame();
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