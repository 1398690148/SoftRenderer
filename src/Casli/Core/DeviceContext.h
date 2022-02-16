#pragma once
#include "CoreAPI.h"
#include "RenderTargetView.h"
#include "VertexBuffer.h"
#include "Math/Math.h"
#include "InputLayout.h"

class IVertexShader;
class IPixelShader;

class CORE_API DeviceContext
{
	friend class RenderTargetView;
public:
	DeviceContext();
	~DeviceContext();
	void ClearRenderTargetView(RenderTargetView *RenderTargetView, const float ColorRGBA[4]);
	void IASetVertexBuffers(IBuffer *buf, const unsigned int *pStrides, const unsigned int *pOffsets);
	void IASetIndexBuffer(IBuffer *buf, unsigned int Offset);
	void VSSetShader(IVertexShader *shader);
	void PSSetShader(IPixelShader *shader);
	void IASetInputLayout(InputLayout *InputLayout);
	void IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology);
	void RSSetViewports(unsigned int NumViewports, const VIEWPORT *Viewports);
	void OMSetRenderTargets(RenderTargetView **RenderTargetView);
	void DrawIndex();

private:
	//void triangle(Vec3f *pts, Vec4f *color);
	void triangle(Vec2i t0, Vec2i t1, Vec2i t2, Vec4f color[3]);
	void triangle(Vec3i t0, Vec3i t1, Vec3i t2, Vec4f color);

	bool ParseVertexBuffer(std::string semanticName, int &offset, int &num, int &size);
private:
	IBuffer *pVertexBuffer{};
	IBuffer *pIndexBuffer{};
	InputLayout *pInputLayout{};
	PRIMITIVE_TOPOLOGY pTopology;
	IVertexShader *pVertexShader{};
	IPixelShader *pPixelShader{};
	RenderTargetView *pRenderTargetView{};
	const VIEWPORT *pViewports{};
};