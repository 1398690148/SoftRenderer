#include "App.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "AlphaBlendVS.h"
#include "AlphaBlendPS.h"
#include "SampleTextureVS.h"
#include "SampleTexturePS.h"

glm::vec3 eye(0, 0, 5);

App::App()
	: wnd(800, 600, "The Donkey Fart Box"), camera(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45)
{
	glm::mat4 Projection = glm::perspective(glm::radians(30.f), 4.0f / 3.0f, 0.1f, 100.f);
	wnd.Gfx().SetProjection(Projection);

	glm::mat4 Model = glm::mat4(1.0);
	Model = glm::scale(Model, glm::vec3(2, 2, 1));
	planeCBuffer.push_back(Projection * camera.GetMatrix() * Model);
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
	wnd.Gfx().SetCamera(camera.GetMatrix());
	wnd.Gfx().SetVertexShader(new SampleTextureVS());
	wnd.Gfx().SetPixelShader(new SampleTexturePS());
	wnd.Gfx().BeginFrame(1.f, 1.f, 1.f);
	model.Draw(wnd.Gfx());
	wnd.Gfx().SetVertexShader(new AlphaBlendVS());
	wnd.Gfx().SetPixelShader(new AlphaBlendPS());
	plane.Bind(wnd.Gfx(), (unsigned char *)(&planeCBuffer[0]));
	plane.Draw(wnd.Gfx());
	wnd.Gfx().EndFrame();
}