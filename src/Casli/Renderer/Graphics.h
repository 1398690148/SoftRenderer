#pragma once
#include <SRDevice.h>
#include <SRDeviceContext.h>
#include <SRRenderTargetView.h>
#include <SRDepthStencilView.h>

class Graphics
{
	friend class Bindable;
public:
	Graphics(unsigned int width, unsigned int height, HWND hWnd, HDC ghdcMainWnd, void *gFbo);
	~Graphics();
	void BeginFrame(float red, float green, float blue);
	void EndFrame();

	void DrawIndexed();

	void SetCamera(glm::mat4x4 cam);
	const glm::mat4 GetCamera();
	void SetProjection(glm::mat4x4 proj);
	const glm::mat4 &GetProjection() const;

private:
	void ClearBuffer(float red, float green, float blue);

private:
	glm::mat4x4 camera;
	glm::mat4x4 projection;
	std::unique_ptr<SRDevice> pDevice;
	std::unique_ptr<SRDeviceContext> pContext;
	SRRenderTargetView *pTarget{};
	SRDepthStencilView *pDSV{};

	int width;
	int height;

	HWND hWnd;
	HDC ghdcMainWnd;
};