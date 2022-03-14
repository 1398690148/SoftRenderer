#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"
#include "Utils.h"
#include <vector>
#include <unordered_map>

class IVertexShader;
class IPixelShader;
class DepthStencilView;
class InputLayout;
class RenderTargetView;
class Texture2D;
class SamplerState;
class BlendState;

class CORE_API DeviceContext
{
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
	void VSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, IBuffer *constantBuffer);
	void PSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, IBuffer *constantBuffer);
	void IASetInputLayout(InputLayout *InputLayout);
	void IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology);
	void RSSetViewports(unsigned int NumViewports, const VIEWPORT *Viewports);
	void OMSetRenderTargets(RenderTargetView **RenderTargetView, DepthStencilView **DepthStencilView);
	void OMSetBlendState(BlendState **blendState, const float *BlendFactor, unsigned int SampleMask);
	void OMGetBlendState(BlendState **ppBlendState, float *BlendFactor, unsigned int *SampleMask);
	void SetRenderState(ShaderState state);
	void GenerateMips(Texture2D *texture);
	void DrawIndex();

private:
	void Triangle(std::vector<glm::vec4> vertex[3]);
	//背面剔除
	bool shouldCulled(const glm::ivec2 &v0, const glm::ivec2 &v1, const glm::ivec2 &v2);

	void ParseVertexBuffer();
	void ParseShaderOutput(unsigned char *buffer, std::vector<glm::vec4> &output);
	
	void ViewportTransform(std::vector<glm::vec4> vertex[3]);
	unsigned char *Vertex(int idx, unsigned char *vertexBuffer);
	void DDXDDY(std::vector<glm::vec4> vertex[3], glm::ivec3 &t0, glm::ivec3 &t1, glm::ivec3 &t2, glm::ivec2 &P);
	void prePerspCorrection(std::vector<glm::vec4> &output);
	unsigned char * Interpolation(std::vector<glm::vec4> vertex[3], glm::vec3 &bcScreen);

	void AlphaBlend(int x, int y, glm::vec4 &color);
	void ParseSrcBlendParam(BLEND blend, glm::vec4 &srcColor, glm::vec4 dstColor);
	void ParseDstBlendParam(BLEND blend, glm::vec4 srcColor, glm::vec4 &dstColor);
private:
	IBuffer *pVertexBuffer{};
	IBuffer *pIndexBuffer{};
	std::vector<std::vector<unsigned int>> indices;
	InputLayout *pInputLayout{};
	PRIMITIVE_TOPOLOGY pTopology;
	IVertexShader *pVertexShader{};
	IPixelShader *pPixelShader{};
	RenderTargetView *pRenderTargetView{};
	DepthStencilView *pDepthStencilView{};
	DepthStencilView *pDSV{};
	VIEWPORT *pViewports{};
	ShaderState pShaderState;
	//混合状态
	BlendState *pBlendState{};
	float pBlendFactor[4];
	unsigned int pSampleMask = 0;
	glm::mat4 Viewport;
	//顶点缓存中的数据
	std::unordered_map<std::string, std::vector<glm::vec4>> m_Data;
	std::unordered_map<std::string, int> pixelInMapTable;
	int posIdx = -1;
	float *frameBuffer{};
	float testW = 0;
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