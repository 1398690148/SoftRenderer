#include "Graphics.h"
#include "../Renderer/LambertVertexShader.h"
#include "../Renderer/LambertPixelShader.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Sampler.h"
#include "../Renderer/VertexConstantBuffer.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 light_dir(1, 1, 1);

Graphics::Graphics(unsigned int width, unsigned int height, HWND hWnd, HDC ghdcMainWnd, void *gFbo)
	: width(width), height(height), hWnd(hWnd), ghdcMainWnd(ghdcMainWnd)
{
	pDevice = std::make_unique<Device>();
	pContext = std::make_unique<DeviceContext>();
	pDevice->CreateRenderTargetView(gFbo, width, height, &pTarget);

	Texture2D *pDepthStencil{};
	TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
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
	pViewports.Width = 800;
	pViewports.Height = 600;
	pViewports.TopLeftX = 0;
	pViewports.TopLeftY = 0;
	pViewports.MaxDepth = 10000;
	pViewports.MinDepth = 0;
	pContext->RSSetViewports(1, &pViewports);

	sampler = new Sampler(*this, 0);
	pPixelShader = new LambertPixelShader();
	pVertexShader = new LambertVertexShader();
}

Graphics::~Graphics()
{
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget, color);
	pContext->ClearDepthStencilView(pDSV);
}

void Graphics::BeginFrame(float red, float green, float blue)
{
	ClearBuffer(0.5f, 0.5f, 0.5f);
	pContext->PSSetShader(pPixelShader);
	pContext->VSSetShader(pVertexShader);

	//允许四种语义Position、Color、Normal、UV
	const INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", sizeof(float), 12, 0},
		{"Normal", sizeof(float), 12, 12},
		{"TEXCOORD0", sizeof(float), 8, 24},
	};
	pDevice->CreateInputLayout(ied, (unsigned int)std::size(ied), &pInputLayout);
	pContext->IASetInputLayout(pInputLayout);

	glm::mat4 matrix = glm::mat4(1.0);
	//matrix = glm::scale(matrix, glm::vec3(3, 3, 1));
	//matrix = glm::rotate(matrix, glm::radians(-60.f), glm::vec3(1.f, 0.f, 0.f));
	//matrix = glm::translate(matrix, glm::vec3(0.f, -7.5f, -10.0f));
	//matrix = projection * camera * matrix;
	//pConstantBuffer = new VertexConstantBuffer(*this, matrix);
	//pConstantBuffer->Bind(*this);

	//tex->Bind(*this);
	sampler->Bind(*this);
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

const glm::mat4 &Graphics::GetCamera() const
{
	return camera;;
}

void Graphics::SetProjection(glm::mat4x4 proj)
{
	projection = proj;
}

const glm::mat4 &Graphics::GetProjection() const
{
	return projection;
}

