#pragma once
#include "Device.h"
#include "DeviceContext.h"
#include "RenderTargetView.h"
#include "./Platform/CasliWin.h"

class ConstantBuffer;
class InputLayout;
class Texture;
class Sampler;

class Graphics
{
	friend class Bindable;
public:
	Graphics(unsigned int width, unsigned int height, HWND hWnd, HDC ghdcMainWnd, void *gFbo);
	~Graphics();
	void BeginFrame(float red, float green, float blue);
	void EndFrame();

	void Draw();

	void SetCamera(glm::mat4x4 cam);
	void SetProjection(glm::mat4x4 proj);
private:
	void ClearBuffer(float red, float green, float blue);

private:
	glm::mat4x4 camera;
	glm::mat4x4 projection;
	std::unique_ptr<Device> pDevice;
	std::unique_ptr<DeviceContext> pContext;
	RenderTargetView *pTarget{};
	DepthStencilView *pDSV{};
	int width;
	int height;
	float *shadowbuffer;

	HWND hWnd;
	HDC ghdcMainWnd;

	ConstantBuffer *pConstantBuffer{};
	InputLayout *pInputLayout;
	Sampler *sampler{};
	IPixelShader *pPixelShader{};
	IVertexShader *pVertexShader{};
	Texture *tex{};
};