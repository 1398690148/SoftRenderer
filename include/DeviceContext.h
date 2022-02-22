#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <vector>
#include <unordered_map>

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
	void ClearDepthStencilView(DepthStencilView *DepthStencilView);
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

	void GenerateMips(Texture2D *texture);

	void DrawIndex();

private:
	glm::vec3 Barycentric(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 P);
	void Triangle(std::unordered_map<std::string, glm::vec4> &o1, std::unordered_map<std::string, glm::vec4> &o2, std::unordered_map<std::string, glm::vec4> &o3);

	void ParseVertexBuffer();
	void ParseShaderOutput(unsigned char *buffer, std::unordered_map<std::string, glm::vec4> &output);

	void viewport(int x, int y, int w, int h);
	void ViewportTransform(glm::vec4 &t0, glm::vec4 &t1, glm::vec4 &t2);
	unsigned char *Vertex(int idx, unsigned char *vertexBuffer);
private:
	IBuffer *pVertexBuffer{};
	IBuffer *pIndexBuffer{};
	std::vector<unsigned int> indices;
	InputLayout *pInputLayout{};
	PRIMITIVE_TOPOLOGY pTopology;
	IVertexShader *pVertexShader{};
	IPixelShader *pPixelShader{};
	RenderTargetView *pRenderTargetView{};
	DepthStencilView *pDepthStencilView{};
	DepthStencilView *pDSV{};
	VIEWPORT *pViewports{};

	glm::mat4 Viewport;

	//顶点缓存中的数据
	std::unordered_map<std::string, std::vector<glm::vec4>> m_Data;
};

class QuadFragments
{
public:
	glm::vec3 m_fragments[4];
	inline float dUdx() const { return m_fragments[1].x - m_fragments[0].x; }
	inline float dUdy() const { return m_fragments[2].x - m_fragments[0].x; }
	inline float dVdx() const { return m_fragments[1].y - m_fragments[0].y; }
	inline float dVdy() const { return m_fragments[2].y - m_fragments[0].y; }
};