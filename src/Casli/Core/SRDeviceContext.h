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
#include <unordered_map>

typedef struct MSAAData
{
	bool coverage[4];
	float depth[4];
};

class CORE_API SRDeviceContext
{
	using VertexData = std::vector<glm::vec4>;
	using TriangleData = std::vector<VertexData>;
public:
	SRDeviceContext(void *gFbo, int width, int height);
	~SRDeviceContext();
	void ClearRenderTargetView(SRRenderTargetView *RenderTargetView, const glm::vec4 &ColorRGBA);
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
	void SetRenderState(RenderState state);
	void GenerateMips(SRTexture2D *texture);
	void DrawIndex();
	//MSAA
	void Resolve();
private:
	void DrawTriangle(TriangleData &vertex);
	//背面剔除
	bool shouldCulled(const glm::ivec2 &v0, const glm::ivec2 &v1, const glm::ivec2 &v2);
	//Clip in the homogeneous clipping space
	std::vector<std::vector<glm::vec4>> ClipSutherlandHodgeman(const std::vector<glm::vec4> &vertex0,
		const std::vector<glm::vec4> &vertex1, const std::vector<glm::vec4> &vertex2, const float Near, const float Far);
	std::vector<std::vector<glm::vec4>> ClipSutherlandHodgemanAux(const std::vector<std::vector<glm::vec4>> &polygon, const int axis
		, const int side);

	void ParseVertexBuffer();
	unsigned char *Vertex(int idx, unsigned char *vertexBuffer);
	void ParseShaderOutput(unsigned char *buffer, VertexData &output);

	void ViewportTransform(TriangleData &vertex);
	void PrePerspCorrection(TriangleData &output);
	//Alpha Blend
	void AlphaBlend(glm::vec4 &color, glm::vec4 dstColor);
	void ParseSrcBlendParam(BLEND blend, glm::vec4 &srcColor, glm::vec4 dstColor);
	void ParseDstBlendParam(BLEND blend, glm::vec4 srcColor, glm::vec4 &dstColor);

	void BindConstanBuffer();
private:
	SRIBuffer *pVertexBuffer{};
	//顶点缓存中的数据
	std::unordered_map<std::string, std::vector<glm::vec4>> m_Data;
	SRIBuffer *pIndexBuffer{};
	std::vector<std::vector<unsigned int>> indices;
	SRInputLayout *pInputLayout{};
	PRIMITIVE_TOPOLOGY pTopology;
	SRIVertexShader *pVertexShader{};
	SRIPixelShader *pPixelShader{};
	SRIBuffer *pPixelConstantBuffer{};
	SRIBuffer *pVertexConstantBuffer{};
	SRRenderTargetView *pBackBuffer{};
	unsigned char *colorBuffer;
	SRDepthStencilView *pDepthStencilView{};
	VIEWPORT *pViewports{};
	RenderState pRenderState;
	//混合状态
	SRBlendState *pBlendState{};
	float pBlendFactor[4];
	unsigned int pSampleMask = 0;
	glm::mat4 Viewport;
	std::unordered_map<std::string, int> vertexOutMapTable;
	int posIdx = -1;
	float frameTime = 0;
};
