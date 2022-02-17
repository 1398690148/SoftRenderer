#include "Graphics.h"
#include "VertexBuffer.h"
#include "Casli/Renderer/LambertVertexShader.h"
#include "Casli/Renderer/LambertPixelShader.h"
#include "Texture2D.h"
#include "Casli/Renderer/Texture.h"
#include "SamplerState.h"

Vec3f light_dir(1, 1, 1);

Graphics::Graphics(unsigned int width, unsigned int height, HWND hWnd, HDC ghdcMainWnd, void *gFbo)
	: width(width), height(height), hWnd(hWnd), ghdcMainWnd(ghdcMainWnd)
{
	pDevice = std::make_unique<Device>();
	pContext = std::make_unique<DeviceContext>();
	pDevice->CreateRenderTargetView(gFbo, width, height, &pTarget);
}

Graphics::~Graphics()
{
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget, color);
}

void Graphics::EndFrame()
{
	HDC hDC = GetDC(hWnd);
	BitBlt(hDC, 0, 0, width, height, ghdcMainWnd, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hDC);
}

void Graphics::Draw()
{
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
		struct
		{
			float u;
			float v;
		} uv;
	};

	Vertex vertices[] =
	{
		{ -0.5, -0.5, -0.5, 1.0, 0, 0, 255, 255, 0, 0 },
		{ 0.5, -0.5, -0.5, 1.0, 0, 0, 255, 255, 1, 0 },
		{ -0.5, 0.5, -0.5, 1.0, 0, 0, 255, 255, 0, 1 },
		{ 0.5, 0.5, -0.5, 1.0, 0, 0, 255, 255, 1, 1 },
		{ -0.5, -0.5, 0.5, 1.0, 255, 0, 0, 255, 0, 0 },
		{ 0.5, -0.5, 0.5, 1.0, 255, 0, 0, 255, 1, 0 },
		{ -0.5, 0.5, 0.5, 1.0, 255, 0, 0, 255, 0, 1 },
		{ 0.5, 0.5, 0.5, 1.0, 255, 0, 0, 255, 1, 1 },
	};

	IBuffer *pVertexBuffer{};
	BUFFER_DESC bd = {};
	bd.BindFlags = BufferType::BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(pVertexBuffer, &stride, &offset);

	// create index buffer
	const unsigned int indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	IBuffer *pIndexBuffer;
	BUFFER_DESC ibd = {};
	ibd.BindFlags = BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned int);
	SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	pContext->IASetIndexBuffer(pIndexBuffer, 0);

	IPixelShader *pPixelShader = new LambertPixelShader();
	pContext->PSSetShader(pPixelShader);

	IVertexShader *pVertexShader = new LambertVertexShader();
	pContext->VSSetShader(pVertexShader);

	//允许四种语义Position、Color、Normal、UV
	const INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", sizeof(float), 16, 0}, 
		{"Color", sizeof(unsigned char), 4, 16},
		{"UV", sizeof(float), 8, 20},
	};
	InputLayout *pInputLayout;
	pDevice->CreateInputLayout(ied, (unsigned int)std::size(ied), &pInputLayout);
	pContext->IASetInputLayout(pInputLayout);

	//Matrix matrixX = Matrix::identity();
	//matrixX[1][1] = cos(PI / 6);
	//matrixX[1][2] = sin(PI / 6);
	//matrixX[2][1] = -sin(PI / 6);
	//matrixX[2][2] = cos(PI / 6);
	//Matrix matrixY = Matrix::identity();
	//matrixY[0][0] = cos(PI / 6);
	//matrixY[0][2] = -sin(PI / 6);
	//matrixY[2][0] = sin(PI / 6);
	//matrixY[2][2] = cos(PI / 6);
	Matrix matrix = Matrix::identity();
	matrix[0][0] = matrix[1][1] = 2;

	matrix = projection * camera * matrix;
	IBuffer *pConstantBuffer{};
	BUFFER_DESC constantDesc = {};
	constantDesc.BindFlags = BIND_CONSTANT_BUFFER;
	constantDesc.ByteWidth = sizeof(Matrix);
	constantDesc.StructureByteStride = sizeof(Matrix);
	SUBRESOURCE_DATA constantSD;
	constantSD.pSysMem = &matrix;
	pDevice->CreateBuffer(&constantDesc, &constantSD, &pConstantBuffer);
	pContext->VSSetConstantBuffers(pConstantBuffer);

	//Set Texture
	Texture tex("../src/Casli/Image/test.png");
	TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = tex.GetWidth();
	textureDesc.Height = tex.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = FORMAT_UNSIGNEDCHAR;
	textureDesc.BindFlags = BIND_SHADER_RESOURCE;

	SUBRESOURCE_DATA texSd = {};
	texSd.pSysMem = tex.GetBufferPtr();
	texSd.SysMemPitch = tex.GetWidth() * tex.GetChannel();
	Texture2D *pTexture;
	pDevice->CreateTexture2D(&textureDesc, &texSd, &pTexture);
	pContext->PSSetShaderResources(0, &pTexture);

	SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = FILTER_LINEAR;
	samplerDesc.AddressU = TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = TEXTURE_ADDRESS_WRAP;
	SamplerState *pSampler{};
	pDevice->CreateSamplerState(&samplerDesc, &pSampler);
	pContext->PSSetSamplerState(0, &pSampler);

	pContext->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VIEWPORT pViewports;
	pViewports.Width = 800;
	pViewports.Height = 600;
	pViewports.TopLeftX = 0;
	pViewports.TopLeftY = 0;
	pContext->RSSetViewports(1, &pViewports);

	Texture2D *pDepthStencil;
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
	pContext->DrawIndex();
	delete pVertexBuffer;
	delete pIndexBuffer;
	delete pInputLayout;
	delete pVertexShader;
	delete pPixelShader;
	delete pDepthStencil;
}

void Graphics::SetCamera(Matrix cam)
{
	camera = cam;
}

void Graphics::SetProjection(Matrix proj)
{
	projection = proj;
}

