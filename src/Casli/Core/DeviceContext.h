#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <map>
#include <vector>

class IVertexShader;
class IPixelShader;
class DepthStencilView;
class InputLayout;
class RenderTargetView;
class Texture2D;
class SamplerState;

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
	void PSSetShaderResources(int slot, Texture2D **texture);
	void PSSetSamplerState(int slot, SamplerState **sampler);
	void VSSetConstantBuffers(IBuffer *constantBuffer);
	void IASetInputLayout(InputLayout *InputLayout);
	void IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology);
	void RSSetViewports(unsigned int NumViewports, const VIEWPORT *Viewports);
	void OMSetRenderTargets(RenderTargetView **RenderTargetView, DepthStencilView **DepthStencilView);
	void DrawIndex();

private:
	glm::vec3 barycentric(glm::i32vec2 A, glm::i32vec2 B, glm::i32vec2 C, glm::i32vec2 P);
	void triangle(unsigned char *o1, unsigned char *o2, unsigned char *o3);

	void ParseVertexBuffer();

	void ParseShaderOutput(unsigned char *buffer, std::vector<glm::vec4> &output);

	void viewport(int x, int y, int w, int h);
private:
	IBuffer *pVertexBuffer{};
	IBuffer *pIndexBuffer{};
	InputLayout *pInputLayout{};
	PRIMITIVE_TOPOLOGY pTopology;
	IVertexShader *pVertexShader{};
	IPixelShader *pPixelShader{};
	RenderTargetView *pRenderTargetView{};
	DepthStencilView *pDepthStencilView{};
	DepthStencilView *pDSV{};
	const VIEWPORT *pViewports{};

	glm::mat4x4 Viewport;

	//顶点缓存中可能包含的数据
	typedef struct Attribute
	{
		std::vector<glm::vec4> data;
		int Size;
		int Offset;
	} Attribute;
	std::map<std::string, Attribute> m_Data;

	int SV_PositionIndex;
};