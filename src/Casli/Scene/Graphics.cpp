#include "Graphics.h"
#include "VertexBuffer.h"

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
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};

	Vertex vertices[] =
	{
		{ 0.5f,0.75f,255,0,0,255 },
		{ 0.75f,0.25f,0,255,0,255 },
		{ 0.25f,0.25f,0,0,255,255 },
		{ 0.35f,0.65f,0,255,0,255 },
		{ 0.65f,0.65f,0,0,255,255 },
		{ 0.5f,0.1f,255,0,0,255 },
	};

	IBuffer *pVertexBuffer;
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
		0,1,2,
		//0,2,3,
		//0,4,1,
		//2,1,5,
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

	const INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", sizeof(float), 8, 0}, 
		{"Color", sizeof(unsigned char), 4, 8}
	};
	InputLayout *pInputLayout;
	pDevice->CreateInputLayout(ied, (unsigned int)std::size(ied), &pInputLayout);
	pContext->IASetInputLayout(pInputLayout);

	pContext->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VIEWPORT pViewports;
	pViewports.Width = 800;
	pViewports.Height = 600;
	pViewports.TopLeftX = 0;
	pViewports.TopLeftY = 0;
	pContext->RSSetViewports(1, &pViewports);

	pContext->OMSetRenderTargets(&pTarget);
	pContext->DrawIndex();

}

