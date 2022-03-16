#include <Graphics.h>
#include <VertexConstantBuffer.h>
#include "PixelConstantBuffer.h"

glm::vec3 light_dir(1, 1, 1);

Graphics::Graphics(unsigned int width, unsigned int height, HWND hWnd, HDC ghdcMainWnd, void *gFbo)
	: width(width), height(height), hWnd(hWnd), ghdcMainWnd(ghdcMainWnd)
{
	pDevice = std::make_unique<SRDevice>();
	pContext = std::make_unique<SRDeviceContext>();
	pDevice->CreateRenderTargetView(gFbo, width, height, &pTarget);

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
	descDSV.Format = FORMAT_FLOAT;
	pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDSV);
	pContext->OMSetRenderTargets(&pTarget, &pDSV);

	VIEWPORT pViewports;
	pViewports.Width = 666;
	pViewports.Height = 500;
	pViewports.TopLeftX = 0;
	pViewports.TopLeftY = 0;
	pViewports.MaxDepth = 10000;
	pViewports.MinDepth = 0;
	pContext->RSSetViewports(1, &pViewports);
}

Graphics::~Graphics()
{
	delete pTarget;
	delete pDSV;
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget, color);
	pContext->ClearDepthStencilView(pDSV);
}

void Graphics::BeginFrame(float red, float green, float blue)
{
	ClearBuffer(red, green, blue);

	//允许四种语义Position、Color、Normal、UV
	const INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", sizeof(float), 12, 0},
		{"Normal", sizeof(float), 12, 12},
		{"TEXCOORD0", sizeof(float), 8, 24},
	};
	pDevice->CreateInputLayout(ied, (unsigned int)std::size(ied), &pInputLayout);
	pContext->IASetInputLayout(pInputLayout);

	pContext->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY::PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::EndFrame()
{
	HDC hDC = GetDC(hWnd);
	BitBlt(hDC, 0, 0, width, height, ghdcMainWnd, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hDC);
}

void Graphics::Draw()
{
	pContext->DrawIndex();
}

void Graphics::SetCamera(glm::mat4x4 cam)
{
	camera = cam;
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

