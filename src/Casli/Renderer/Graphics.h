#pragma once
#include <SRDevice.h>
#include <RendererAPI.h>
#include <SRDeviceContext.h>
#include <SRRenderTargetView.h>
#include <SRDepthStencilView.h>
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
	void SetProjection(glm::mat4x4 proj);
	const glm::mat4 &GetProjection() const;

	void SetVertexShader(SRIVertexShader *pVertexShader);
	void SetPixelShader(SRIPixelShader *pPixelShader);
private:
	void ClearBuffer(float red, float green, float blue);

private:
	glm::mat4x4 camera;
	glm::mat4x4 projection;
	std::unique_ptr<SRDevice> pDevice;
	std::unique_ptr<SRDeviceContext> pContext;
	SRRenderTargetView *pTarget{};
	SRDepthStencilView *pDSV{};

	unsigned char *pPixelConstantBuffer{};
	unsigned int pPixelOffset;
	
	int width;
	int height;
	

	HWND hWnd;
	HDC ghdcMainWnd;
	SRInputLayout *pInputLayout;
};