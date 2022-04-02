#include <Graphics.h>

Graphics::Graphics(unsigned int width, unsigned int height, HWND hWnd, HDC ghdcMainWnd, void *gFbo)
	: width(width), height(height), hWnd(hWnd), ghdcMainWnd(ghdcMainWnd)
{
	pDevice = std::make_unique<SRDevice>();
	pContext = std::make_unique<SRDeviceContext>(gFbo, 4);
	pDevice->CreateRenderTargetView(width * 2, height * 2, &pTarget);

	SRTexture2D *pDepthStencil{};
	TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 666u;
	descDepth.Height = 500u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = FORMAT_FLOAT;
	descDepth.BindFlags = BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.width = 666u;
	descDSV.height = 500u;
	descDSV.Format = FORMAT_FLOAT;
	pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDSV);
	pContext->OMSetRenderTargets(&pTarget, &pDSV);

	VIEWPORT pViewports;
	pViewports.Width = 666;
	pViewports.Height = 500;
	pViewports.TopLeftX = 0;
	pViewports.TopLeftY = 0;
	pContext->RSSetViewports(1, &pViewports);
}

Graphics::~Graphics()
{
	delete pTarget;
	delete pDSV;
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	glm::vec4 color = glm::vec4(red, green, blue, 255);
	pContext->ClearRenderTargetView(pTarget, color);
	pContext->ClearDepthStencilView(pDSV);
}

void Graphics::BeginFrame(float red, float green, float blue)
{
	ClearBuffer(red, green, blue);
}

void Graphics::EndFrame()
{
	pContext->Resolve();
	HDC hDC = GetDC(hWnd);
	BitBlt(hDC, 0, 0, width, height, ghdcMainWnd, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hDC);
}

void Graphics::DrawIndexed()
{
	pContext->DrawIndex();
}

void Graphics::SetCamera(glm::mat4x4 cam)
{
	camera = cam;
}

const glm::mat4 Graphics::GetCamera()
{
	return camera;
}

void Graphics::SetProjection(glm::mat4x4 proj)
{
	projection = proj;
}

const glm::mat4 &Graphics::GetProjection() const
{
	return projection;
}

void Graphics::SetVertexShader(SRIVertexShader *pVertexShader)
{
	pContext->VSSetShader(pVertexShader);
}

void Graphics::SetPixelShader(SRIPixelShader *pPixelShader)
{
	pContext->PSSetShader(pPixelShader);
}

