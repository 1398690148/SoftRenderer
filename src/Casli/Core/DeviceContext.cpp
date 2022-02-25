﻿#include <algorithm>
#include "DeviceContext.h"
#include "IVertexShader.h"
#include "IPixelShader.h"
#include "InputLayout.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

DeviceContext::DeviceContext()
{
	tempBuffer = new unsigned char[160];
}

DeviceContext::~DeviceContext()
{
	delete tempBuffer;
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
		indices.push_back(indexBuffer[0]);
		offset += pIndexBuffer->GetStructureByteStride();
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

void DeviceContext::VSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, IBuffer *constantBuffer)
{
	if (!constantBuffer) return;
	pVertexShader->cbuffer = (unsigned char *)realloc(pVertexShader->cbuffer, StartOffset + constantBuffer->GetByteWidth()); //new unsigned char[constantBuffer->GetByteWidth()];
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
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		unsigned char *o0 = Vertex(i, tempBuffer);
		unsigned char *o1 = Vertex(i + 1, tempBuffer);
		unsigned char *o2 = Vertex(i + 2, tempBuffer);

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
	}
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
	for (int i = 0; i < total_height; i++) 
	{
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
		glm::vec3 A = t0 + (t2 - t0) * alpha;
		glm::vec3 B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) 
		{
			glm::vec2 P(j, t0.y + i);
			//执行裁剪
			if ((j < 0 && B.x < 0) || P.y < 0 || j >= pViewports->Width || P.y >= pViewports->Height) break;
			if (j < 0) continue;
			glm::vec3 bcScreen = Barycentric(t0, t1, t2, P);
			//插值深度
			float depth = BarycentricLerp(t0, t1, t2, bcScreen).z;
			if (bcScreen[0] < 0 || bcScreen[1] < 0 || bcScreen[2] < 0 || pDepthStencilView->GetDepth(j, t0.y + i) - 0.0001f < depth)
			{
				continue;
			}
			if (vertex[0].find("SV_TEXCOORD0") != vertex[0].end())
			{
				DDXDDY(vertex, t0, t1, t2, P);
			}
			Interpolation(vertex, bcScreen);
			glm::vec4 colors(0, 0, 0, 0);
			bool discard = pPixelShader->fragment(tempBuffer, colors);
			if (pDepthStencilView->GetDepth(j, t0.y + i) - 0.0001f < depth)
			{
				continue;
			}
			pRenderTargetView->SetPixel(P.x, P.y, colors[0], colors[1], colors[2], colors[3]);
			pDepthStencilView->SetDepth(j, t0.y + i, depth);
		}
	}
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
			glm::vec4 v;
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
		memcpy(vertexBuffer + pVertexShader->inDesc[i].Offset, &attr[indices[idx]], pVertexShader->inDesc[i].Size);
	}
	return pVertexShader->vertex(vertexBuffer);
}

void DeviceContext::DDXDDY(std::unordered_map<std::string, glm::vec4> vertex[3], glm::vec3 &t0, glm::vec3 &t1, glm::vec3 &t2, glm::vec2 &P)
{
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
	memcpy(&pPixelShader->dFdx, &ddx, sizeof(glm::vec2));
	memcpy(&pPixelShader->dFdy, &ddy, sizeof(glm::vec2));
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

void DeviceContext::Interpolation(std::unordered_map<std::string, glm::vec4> vertex[3], glm::vec3 &bcScreen)
{
	//插值其他信息
	for (auto iter : pPixelShader->inDesc)
	{
		glm::vec4 attribute = vertex[0][iter.Name] * bcScreen[0] + vertex[1][iter.Name] * bcScreen[1] + vertex[2][iter.Name] * bcScreen[2];
		if (iter.Name == "SV_TEXCOORD0" || iter.Name == "SV_Normal")
		{
			attribute /= attribute.w;
		}
		memcpy(tempBuffer + iter.Offset, &attribute, iter.Size);
	}
}
