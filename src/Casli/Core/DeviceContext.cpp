#include <algorithm>
#include "DeviceContext.h"
#include "IVertexShader.h"
#include "IPixelShader.h"
#include "InputLayout.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "BlendState.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/blocked_range2d.h"
#include <iostream>
#include <execution>
#include <algorithm>

DeviceContext::DeviceContext()
{
	BLEND_DESC desc = { 0 };
	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].SrcBlend = BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = 0;
	pBlendState = new BlendState(&desc);
}

DeviceContext::~DeviceContext()
{
}

void DeviceContext::ClearRenderTargetView(RenderTargetView *RenderTargetView, const float ColorRGBA[4])
{
	RenderTargetView->ClearBuffer(ColorRGBA);
}

void DeviceContext::ClearDepthStencilView(DepthStencilView * DepthStencilView)
{
	DepthStencilView->ClearBuffer(FLT_MAX);
}

void DeviceContext::IASetVertexBuffers(IBuffer * buf, const unsigned int *pStrides, const unsigned int *pOffsets)
{
	pVertexBuffer = buf;
}

void DeviceContext::IASetIndexBuffer(IBuffer * buf, unsigned int Offset)
{
	pIndexBuffer = buf;
	indices.clear();
	for (int offset = 0; offset < pIndexBuffer->GetByteWidth();)
	{
		unsigned int *indexBuffer = (unsigned int *)pIndexBuffer->GetBuffer(offset);
		indices.push_back({ indexBuffer[0], indexBuffer[1], indexBuffer[2] });
		offset += pIndexBuffer->GetStructureByteStride() * 3;
	}
}

void DeviceContext::VSSetShader(IVertexShader * shader)
{
	pVertexShader = shader;
}

void DeviceContext::PSSetShader(IPixelShader * shader)
{
	pPixelShader = shader;
}

void DeviceContext::PSSetShaderResources(int slot, Texture2D **texture)
{
	if (slot >= 16) return;
	pPixelShader->textures[slot] = *texture;
}

void DeviceContext::PSSetSamplerState(int slot, SamplerState **sampler)
{
	if (slot >= 4) return;
	pPixelShader->samplers[slot] = *sampler;
}

void DeviceContext::OMSetBlendState(BlendState **blendState, const float *BlendFactor, unsigned int SampleMask)
{
	pBlendState = *blendState;
	if (BlendFactor)
		memcpy(pBlendFactor, BlendFactor, sizeof(float));
}

void DeviceContext::OMGetBlendState(BlendState **ppBlendState, float *BlendFactor, unsigned int *SampleMask)
{
	*ppBlendState = pBlendState;
	BlendFactor = pBlendFactor;
	*SampleMask = pSampleMask;
}

void DeviceContext::VSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, IBuffer *constantBuffer)
{
	if (!constantBuffer) return;
	pVertexShader->cbuffer = (unsigned char *)realloc(pVertexShader->cbuffer, StartOffset + constantBuffer->GetByteWidth()); 
	memcpy(pVertexShader->cbuffer + StartOffset, constantBuffer->GetBuffer(0), constantBuffer->GetByteWidth());
}

void DeviceContext::PSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, IBuffer *constantBuffer)
{
	if (!constantBuffer) return;
	pPixelShader->cbuffer = (unsigned char *)realloc(pPixelShader->cbuffer, StartOffset + constantBuffer->GetByteWidth());
	memcpy(pPixelShader->cbuffer + StartOffset, constantBuffer->GetBuffer(0), constantBuffer->GetByteWidth());
}

void DeviceContext::IASetInputLayout(InputLayout *InputLayout)
{
	pInputLayout = InputLayout;
}

void DeviceContext::IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology)
{
	pTopology = topology;
}

void DeviceContext::RSSetViewports(unsigned int NumViewports, const VIEWPORT *Viewports)
{
	pViewports = new VIEWPORT();
	memcpy(pViewports, Viewports, sizeof(VIEWPORT));
	Viewport = glm::mat4x4(1.0);
	Viewport[3][0] = (pViewports->Width - pViewports->TopLeftX) >> 1;
	Viewport[3][1] = (pViewports->Height - pViewports->TopLeftY) >> 1;
	Viewport[3][2] = (pViewports->MaxDepth - pViewports->MinDepth) >> 1;

	Viewport[0][0] = pViewports->Width >> 1;
	Viewport[1][1] = pViewports->Height >> 1;
	Viewport[2][2] = (pViewports->MaxDepth - pViewports->MinDepth) >> 1;
}

void DeviceContext::OMSetRenderTargets(RenderTargetView **RenderTargetView, DepthStencilView **DepthStencilView)
{
	pRenderTargetView = *RenderTargetView;
	pDepthStencilView = *DepthStencilView;
}

void DeviceContext::GenerateMips(Texture2D *texture)
{
	texture->GenerateMips();
}

void DeviceContext::DrawIndex()
{
	ParseVertexBuffer();
	tbb::parallel_for(0, (int)indices.size(), 1, [&](size_t i)
	{
		unsigned char *buffer = new unsigned char[160];
		unsigned char *o0 = Vertex(indices[i][0], buffer);
		unsigned char *o1 = Vertex(indices[i][1], buffer);
		unsigned char *o2 = Vertex(indices[i][2], buffer);

		std::unordered_map<std::string, glm::vec4> vertex[3];
		ParseShaderOutput(o0, vertex[0]);
		ParseShaderOutput(o1, vertex[1]);
		ParseShaderOutput(o2, vertex[2]);
		if (vertex[0].find("SV_Position") == vertex[0].end()) return;
		//视口变换
		ViewportTransform(vertex);
		Triangle(vertex);
		delete o0;
		delete o1;
		delete o2;
		delete buffer;
	});
	//for (unsigned int i = 0; i < indices.size(); i += 3)
	//{
	//	unsigned char *o0 = Vertex(i, tempBuffer);
	//	unsigned char *o1 = Vertex(i + 1, tempBuffer);
	//	unsigned char *o2 = Vertex(i + 2, tempBuffer);

	//	std::unordered_map<std::string, glm::vec4> vertex[3];
	//	ParseShaderOutput(o0, vertex[0]);
	//	ParseShaderOutput(o1, vertex[1]);
	//	ParseShaderOutput(o2, vertex[2]);
	//	if (vertex[0].find("SV_Position") == vertex[0].end()) return;
	//	//视口变换
	//	ViewportTransform(vertex);
	//	Triangle(vertex);
	//	delete o0;
	//	delete o1;
	//	delete o2;
	//}
}

void DeviceContext::Triangle(std::unordered_map<std::string, glm::vec4> vertex[3])
{
	glm::vec3 t0 = glm::i32vec3(vertex[0]["SV_Position"].x, vertex[0]["SV_Position"].y, vertex[0]["SV_Position"].z);
	glm::vec3 t1 = glm::i32vec3(vertex[1]["SV_Position"].x, vertex[1]["SV_Position"].y, vertex[1]["SV_Position"].z);
	glm::vec3 t2 = glm::i32vec3(vertex[2]["SV_Position"].x, vertex[2]["SV_Position"].y, vertex[2]["SV_Position"].z);

	if (t0.y == t1.y && t0.y == t2.y) return; // I dont care about degenerate triangles 
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	//排序顶点时不要忘记排序其他属性
	if (t0.y > t1.y)
	{
		std::swap(t0, t1);
		std::swap(vertex[0], vertex[1]);
	}
	if (t0.y > t2.y)
	{
		std::swap(t0, t2);
		std::swap(vertex[0], vertex[2]);
	}
	if (t1.y > t2.y)
	{
		std::swap(t1, t2);
		std::swap(vertex[1], vertex[2]);
	}
	int total_height = t2.y - t0.y;
	auto drawTriangle = [&](size_t i)
	{
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
		glm::vec3 A = t0 + (t2 - t0) * alpha;
		glm::vec3 B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j < B.x; j++)
		{
			glm::vec2 P(j, t0.y + i);
			//执行裁剪
			if ((j < 0 && B.x < 0) || P.y < 0 || j >= pViewports->Width || P.y >= pViewports->Height) break;
			if (j < 0) continue;
			glm::vec3 bcScreen = Barycentric(t0, t1, t2, P);
			//插值深度
			float depth = BarycentricLerp(t0, t1, t2, bcScreen).z;
			//Early-Z
			if (bcScreen[0] < 0 || bcScreen[1] < 0 || bcScreen[2] < 0 ||
				(!pBlendState->blendDesc.RenderTarget[0].BlendEnable && pDepthStencilView->GetDepth(j, t0.y + i) - 0.0001f < depth))
			{
				continue;
			}
			DDXDDY(vertex, t0, t1, t2, P);
			unsigned char *buffer = Interpolation(vertex, bcScreen);
			glm::vec4 color(0, 0, 0, 255);
			bool discard = pPixelShader->fragment(buffer, color);
			delete buffer;
			//Alpha Test/Alpha Blend
			if (pBlendState->blendDesc.RenderTarget[0].BlendEnable)
			{
				if (discard)
				{
					continue;
				}
				AlphaBlend(P.x, P.y, color);
				pRenderTargetView->SetPixel(P.x, P.y, color[0], color[1], color[2], color[3]);
				continue;
			}
			if (pDepthStencilView->GetDepth(j, t0.y + i) - 0.0001f < depth)
			{
				continue;
			}
			color = glm::vec4(min(255.0f, color[0]), min(255.0f, color[1]), min(255.0f, color[2]), min(255.0f, color[3]));
			pRenderTargetView->SetPixel(P.x, P.y, color[0], color[1], color[2], color[3]);
			pDepthStencilView->SetDepth(j, t0.y + i, depth);
		}
	};
	tbb::parallel_for(tbb::blocked_range<size_t>(0, total_height), [&](const tbb::blocked_range<size_t> &r)
	{
		for (size_t i = r.begin(); i != r.end(); i++)
		{
			drawTriangle(i);
		}
	});
}

void DeviceContext::ParseVertexBuffer()
{
	int bufferSize = pVertexBuffer->GetByteWidth() / pVertexBuffer->GetStructureByteStride();
	for (int i = 0; i < pInputLayout->getSize(); i++)
	{
		const INPUT_ELEMENT_DESC *desc = pInputLayout->getData(i);
		std::vector<glm::vec4> attr;
		int num = pInputLayout->getData(i)->Size / pInputLayout->getData(i)->typeSize;
		int inOffset = pInputLayout->getData(i)->Offset;
		for (int j = 0; j < bufferSize; j++)
		{
			glm::vec4 v(0, 0, 0, 0);
			v[0] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset));
			v[1] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + desc->typeSize));
			if (num > 2)
			{
				v[2] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 2 * desc->typeSize));
				if (num > 3)
				{
					v[3] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 3 * desc->typeSize));
				}
			}
			attr.push_back(v);
		}
		m_Data[desc->SemanticName] = attr;
	}
}

void DeviceContext::ParseShaderOutput(unsigned char *buffer, std::unordered_map<std::string, glm::vec4> &output)
{
	int offset = 0;
	for (int i = 0; i < pVertexShader->outDesc.size(); i++)
	{
		float *start = (float *)(buffer + offset);
		switch (pVertexShader->outDesc[i].Size / pVertexShader->outDesc[i].typeSize)
		{
		case 2:
		{
			output[pVertexShader->outDesc[i].Name] = glm::vec4((*start), *(start + 1), 1, 1);
			start += 2;
		}
		break;
		case 3:
		{
			output[pVertexShader->outDesc[i].Name] = glm::vec4(*start, *(start + 1), *(start + 2), 1);
			start += 3;
		}
		break;
		case 4:
		{
			output[pVertexShader->outDesc[i].Name] = glm::vec4(*start, *(start + 1), *(start + 2), *(start + 3));
			start += 4;
		}
		break;
		}
		offset += pVertexShader->outDesc[i].Size;
	}
	prePerspCorrection(output);
}

void DeviceContext::ViewportTransform(std::unordered_map<std::string, glm::vec4> vertex[3])
{
	vertex[0]["SV_Position"] = Viewport * vertex[0]["SV_Position"];
	vertex[1]["SV_Position"] = Viewport * vertex[1]["SV_Position"];
	vertex[2]["SV_Position"] = Viewport * vertex[2]["SV_Position"];
}

unsigned char * DeviceContext::Vertex(int idx, unsigned char *vertexBuffer)
{
	for (int i = 0; i < pVertexShader->inDesc.size(); i++)
	{
		const std::vector<glm::vec4> &attr = m_Data[pVertexShader->inDesc[i].Name];
		memcpy(vertexBuffer + pVertexShader->inDesc[i].Offset, &attr[idx], pVertexShader->inDesc[i].Size);
	}
	return pVertexShader->vertex(vertexBuffer);
}

void DeviceContext::DDXDDY(std::unordered_map<std::string, glm::vec4> vertex[3], glm::vec3 &t0, glm::vec3 &t1, glm::vec3 &t2, glm::vec2 &P)
{
	if (vertex[0].find("SV_TEXCOORD0") == vertex[0].end()) return;
	QuadFragments quadFragment;
	glm::vec3 texCoord0 = vertex[0]["SV_TEXCOORD0"];
	glm::vec3 texCoord1 = vertex[1]["SV_TEXCOORD0"];
	glm::vec3 texCoord2 = vertex[2]["SV_TEXCOORD0"];
	quadFragment.m_fragments[0] = BarycentricLerp(texCoord0, texCoord1, texCoord2, Barycentric(t0, t1, t2, glm::vec3(P.x, P.y, 1.0)));
	quadFragment.m_fragments[1] = BarycentricLerp(texCoord0, texCoord1, texCoord2, Barycentric(t0, t1, t2, glm::vec3(P.x + 1, P.y, 1.0)));
	quadFragment.m_fragments[2] = BarycentricLerp(texCoord0, texCoord1, texCoord2, Barycentric(t0, t1, t2, glm::vec3(P.x, P.y + 1, 1.0)));
	quadFragment.m_fragments[3] = BarycentricLerp(texCoord0, texCoord1, texCoord2, Barycentric(t0, t1, t2, glm::vec3(P.x + 1, P.y + 1, 1.0)));
	quadFragment.m_fragments[0] /= quadFragment.m_fragments[0].z;
	quadFragment.m_fragments[1] /= quadFragment.m_fragments[1].z;
	quadFragment.m_fragments[2] /= quadFragment.m_fragments[2].z;
	quadFragment.m_fragments[3] /= quadFragment.m_fragments[3].z;
	glm::vec2 ddx(quadFragment.dUdx(), quadFragment.dUdy());
	glm::vec2 ddy(quadFragment.dVdx(), quadFragment.dVdy());
	memcpy(&pPixelShader->dFdx.local(), &ddx, sizeof(glm::vec2));
	memcpy(&pPixelShader->dFdy.local(), &ddy, sizeof(glm::vec2));
}

void DeviceContext::prePerspCorrection(std::unordered_map<std::string, glm::vec4>& output)
{
	if (output.find("SV_Position") == output.end()) return;
	if (output.find("SV_TEXCOORD0") != output.end())
	{
		output["SV_TEXCOORD0"] /= output["SV_Position"].w;
	}
	if (output.find("SV_Normal") != output.end())
	{
		output["SV_Normal"] /= output["SV_Position"].w;
	}
	output["SV_Position"] /= output["SV_Position"].w;
}

unsigned char *DeviceContext::Interpolation(std::unordered_map<std::string, glm::vec4> vertex[3], glm::vec3 &bcScreen)
{
	//插值其他信息
	unsigned char *buffer = new unsigned char[100];
	for (int i = 0; i < pPixelShader->inDesc.size(); i++)
	{
		glm::vec4 attribute = vertex[0][pPixelShader->inDesc[i].Name] * bcScreen[0] +
			vertex[1][pPixelShader->inDesc[i].Name] * bcScreen[1] + vertex[2][pPixelShader->inDesc[i].Name] * bcScreen[2];
		if (pPixelShader->inDesc[i].Name == "SV_TEXCOORD0" || pPixelShader->inDesc[i].Name == "SV_Normal")
		{
			attribute /= attribute.w;
		}
		memcpy(buffer + pPixelShader->inDesc[i].Offset, &attribute, pPixelShader->inDesc[i].Size);
	}
	return buffer;
}

void DeviceContext::AlphaBlend(int x, int y, glm::vec4 &color)
{
	glm::vec4 dstColor = pRenderTargetView->GetPixel(x, y);
		
	glm::vec4 srcColor = color;
	ParseSrcBlendParam(pBlendState->blendDesc.RenderTarget[0].SrcBlend, color, dstColor);
	ParseDstBlendParam(pBlendState->blendDesc.RenderTarget[0].DestBlend, srcColor, dstColor);
	switch (pBlendState->blendDesc.RenderTarget[0].BlendOp)
	{
	case BLEND_OP_ADD:
		color += dstColor;
		break;
	case BLEND_OP_MAX:
		color = glm::vec4(max(dstColor[0], color[0]), max(dstColor[1], color[1]), max(dstColor[2], color[2]), max(dstColor[3], color[3]));
		break;
	case BLEND_OP_MIN:
		color = glm::vec4(min(dstColor[0], color[0]), min(dstColor[1], color[1]), min(dstColor[2], color[2]), min(dstColor[3], color[3]));
		break;
	default:
		break;
	}
	color = glm::vec4(min(255.0f, color[0]), min(255.0f, color[1]), min(255.0f, color[2]), min(255.0f, color[3]));
}

void DeviceContext::ParseSrcBlendParam(BLEND blend, glm::vec4 &srcColor, glm::vec4 dstColor)
{
	switch (blend)
	{
	case BLEND_ZERO:
		srcColor = glm::vec4(0, 0, 0, 255);
		break;
	case BLEND_ONE:
		srcColor = glm::vec4(255, 255, 255, 255);
		break;
	case BLEND_INV_SRC_COLOR:
		srcColor = glm::vec4(255, 255, 255, 255) - srcColor;
		break;
	case BLEND_SRC_ALPHA:
		srcColor *= (srcColor[3] / 255.f);
		break;
	case BLEND_INV_SRC_ALPHA:
		srcColor *= (1 - srcColor[3] / 255.f);
		break;
	case BLEND_DEST_ALPHA:
		srcColor *= dstColor[3] / 255.f;
		break;
	case BLEND_INV_DEST_ALPHA:
		srcColor *= (1 - dstColor[3] / 255.f);
		break;
	case BLEND_DEST_COLOR:
		srcColor = dstColor;
		break;
	case BLEND_INV_DEST_COLOR:
		srcColor = glm::vec4(255, 255, 255, 255) - dstColor;
		break;
	case BLEND_BLEND_FACTOR:
		srcColor *= pBlendFactor[0];
		break;
	case BLEND_INV_BLEND_FACTOR:
		srcColor *= (1 - pBlendFactor[0]);
		break;
	default:
		break;
	}
}

void DeviceContext::ParseDstBlendParam(BLEND blend, glm::vec4 srcColor, glm::vec4 &dstColor)
{
	switch (blend)
	{
	case BLEND_ZERO:
		dstColor = glm::vec4(0, 0, 0, 255);
		break;
	case BLEND_ONE:
		dstColor = glm::vec4(255, 255, 255, 255);
		break;
	case BLEND_SRC_COLOR:
		dstColor = srcColor;
		break;
	case BLEND_INV_SRC_COLOR:
		dstColor = glm::vec4(255, 255, 255, 255) - srcColor;
		break;
	case BLEND_SRC_ALPHA:
		dstColor *= srcColor[3] / 255.f;
		break;
	case BLEND_INV_SRC_ALPHA:
		dstColor *= (1 - srcColor[3] / 255.f);
		break;
	case BLEND_DEST_ALPHA:
		dstColor *= dstColor[3] / 255.f;
		break;
	case BLEND_INV_DEST_ALPHA:
		dstColor *= (1 - dstColor[3] / 255.f);
		break;
	case BLEND_INV_DEST_COLOR:
		dstColor = glm::vec4(255, 255, 255, 255) - dstColor;
		break;
	case BLEND_BLEND_FACTOR:
		dstColor *= pBlendFactor[0];
		break;
	case BLEND_INV_BLEND_FACTOR:
		dstColor *= (1 - pBlendFactor[0]);
		break;
	default:
		break;
	}
}
