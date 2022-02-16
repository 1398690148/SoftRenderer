#pragma once
#include "Math/Math.h"
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
private:
	Matrix camera;
	Matrix projection;
	Matrix M;

	std::unique_ptr<Device> pDevice;
	std::unique_ptr<DeviceContext> pContext;
	RenderTargetView *pTarget{};
	int width;
	int height;
	float *shadowbuffer;

	HWND hWnd;
	HDC ghdcMainWnd;
};