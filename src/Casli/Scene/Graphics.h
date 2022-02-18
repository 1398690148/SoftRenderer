#pragma once
#include "Device.h"
#include "DeviceContext.h"
#include "RenderTargetView.h"
#include "./Platform/CasliWin.h"
class Graphics
{
public:
	Graphics(unsigned int width, unsigned int height, HWND hWnd, HDC ghdcMainWnd, void *gFbo);
	~Graphics();
	void ClearBuffer(float red, float green, float blue);
	void EndFrame();

	void Draw();

	void SetCamera(glm::mat4x4 cam);
	void SetProjection(glm::mat4x4 proj);
private:
	glm::mat4x4 camera;
	glm::mat4x4 projection;
	int angle = 0;
	std::unique_ptr<Device> pDevice;
	std::unique_ptr<DeviceContext> pContext;
	RenderTargetView *pTarget{};
	DepthStencilView *pDSV{};
	int width;
	int height;
	float *shadowbuffer;

	HWND hWnd;
	HDC ghdcMainWnd;
};