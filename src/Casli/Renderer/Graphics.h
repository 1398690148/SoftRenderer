#pragma once
#include <Device.h>
#include <RendererAPI.h>
#include <DeviceContext.h>
#include <RenderTargetView.h>
#include "Platform/CasliWin.h"

class VertexConstantBuffer;
class InputLayout;
class Texture;
class Sampler;

class RENDERER_API Graphics
{
	friend class Bindable;
	friend class Drawable;
public:
	Graphics(unsigned int width, unsigned int height, HWND hWnd, HDC ghdcMainWnd, void *gFbo);
	~Graphics();
	void BeginFrame(float red, float green, float blue);
	void EndFrame();

	void Draw();

	void SetCamera(glm::mat4x4 cam);
	const glm::mat4 &GetCamera() const;
	void SetProjection(glm::mat4x4 proj);
	const glm::mat4 &GetProjection() const;

	void SetVertexShader(IVertexShader *pVertexShader);
	void SetPixelShader(IPixelShader *pPixelShader);
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
	InputLayout *pInputLayout;
};