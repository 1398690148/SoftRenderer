#pragma once
#include "SRCoreAPI.h"
#include "SRIBuffer.h"
#include "SRRenderTargetView.h"
#include "SRDepthStencilView.h"
#include "SRIVertexShader.h"
#include "SRIPixelShader.h"
#include "SRTexture2D.h"
#include "SRSamplerState.h"
#include "SRInputLayout.h"
#include "SRBlendState.h"
#include "Utils.h"
#include <vector>
#include <unordered_map>

class CORE_API SRDeviceContext
{
public:
	SRDeviceContext(void *gFbo, int width, int height);
	~SRDeviceContext();
	void ClearRenderTargetView(SRRenderTargetView *RenderTargetView, const float ColorRGBA[4]);
	void ClearDepthStencilView(SRDepthStencilView *DepthStencilView);
	void IASetVertexBuffers(SRIBuffer *buf, const unsigned int *pStrides, const unsigned int *pOffsets);
	void IASetIndexBuffer(SRIBuffer *buf, unsigned int Offset);
	void VSSetShader(SRIVertexShader *shader);
	void PSSetShader(SRIPixelShader *shader);
	void PSSetShaderResources(int slot, SRTexture2D **texture);
	void PSSetSamplerState(int slot, SRSamplerState **sampler);
	void VSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, SRIBuffer *constantBuffer);
	void PSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, SRIBuffer *constantBuffer);
	void IASetInputLayout(SRInputLayout *InputLayout);
	void IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology);
	void RSSetViewports(unsigned int NumViewports, const VIEWPORT *Viewports);
	void OMSetRenderTargets(SRRenderTargetView **RenderTargetView, SRDepthStencilView **DepthStencilView);
	void OMSetBlendState(SRBlendState **blendState, const float *BlendFactor, unsigned int SampleMask);
	void OMGetBlendState(SRBlendState **ppBlendState, float *BlendFactor, unsigned int *SampleMask);
	void SetRenderState(ShaderState state);
	void GenerateMips(SRTexture2D *texture);
	void DrawIndex();
	void SwapBuffer();

private:
	void Triangle(std::vector<glm::vec4> vertex[3]);
	//背面剔除
	bool shouldCulled(const glm::ivec2 &v0, const glm::ivec2 &v1, const glm::ivec2 &v2);
	//Clip in the homogeneous clipping space
	std::vector<std::vector<glm::vec4>> ClipSutherlandHodgeman(const std::vector<glm::vec4> &vertex0,
		const std::vector<glm::vec4> &vertex1, const std::vector<glm::vec4> &vertex2, const float Near, const float Far);
	std::vector<std::vector<glm::vec4>> ClipSutherlandHodgemanAux(const std::vector<std::vector<glm::vec4>> &polygon, const int axis
		, const int side);

	void ParseVertexBuffer();
	void ParseShaderOutput(unsigned char *buffer, std::vector<glm::vec4> &output);
	
	void ViewportTransform(std::vector<glm::vec4> vertex[3]);
	unsigned char *Vertex(int idx, unsigned char *vertexBuffer);
	void DDXDDY(std::vector<glm::vec4> vertex[3], glm::vec3 &t0, glm::vec3 &t1, glm::vec3 &t2, glm::ivec2 &P);
	void prePerspCorrection(std::vector<glm::vec4> output[3]);
	unsigned char * Interpolation(std::vector<glm::vec4> vertex[3], glm::vec3 &bcScreen);
	//Alpha Blend
	void AlphaBlend(int x, int y, glm::vec4 &color);
	void ParseSrcBlendParam(BLEND blend, glm::vec4 &srcColor, glm::vec4 dstColor);
	void ParseDstBlendParam(BLEND blend, glm::vec4 srcColor, glm::vec4 &dstColor);

	void BindConstanBuffer();
	//void Resolve();
private:
	SRIBuffer *pVertexBuffer{};
	SRIBuffer *pIndexBuffer{};
	std::vector<std::vector<unsigned int>> indices;
	SRInputLayout *pInputLayout{};
	PRIMITIVE_TOPOLOGY pTopology;
	SRIVertexShader *pVertexShader{};
	SRIPixelShader *pPixelShader{};
	SRIBuffer *pPixelConstantBuffer{};
	SRIBuffer *pVertexConstantBuffer{};
	SRRenderTargetView *pBackBuffer{};
	SRRenderTargetView *pFrontBuffer{};
	SRDepthStencilView *pDepthStencilView{};
	VIEWPORT *pViewports{};
	ShaderState pShaderState;
	//混合状态
	SRBlendState *pBlendState{};
	float pBlendFactor[4];
	unsigned int pSampleMask = 0;
	glm::mat4 Viewport;
	//顶点缓存中的数据
	std::unordered_map<std::string, std::vector<glm::vec4>> m_Data;
	std::unordered_map<std::string, int> vertexOutMapTable;
	int posIdx = -1;
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