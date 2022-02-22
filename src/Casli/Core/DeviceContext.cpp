#include <algorithm>
#include "DeviceContext.h"
#include "IVertexShader.h"
#include "IPixelShader.h"
#include "InputLayout.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

DeviceContext::DeviceContext()
{
	
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

void DeviceContext::VSSetConstantBuffers(IBuffer *constantBuffer)
{
	pVertexShader->buffer = new unsigned char[constantBuffer->GetByteWidth()];
	memcpy(pVertexShader->buffer, constantBuffer->GetBuffer(0), constantBuffer->GetByteWidth());
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
	viewport(0, 0, Viewports->Width, Viewports->Height);
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
	int Size = 0;
	for (int i = 0; i < pVertexShader->inDesc.size(); i++)
	{
		Size += pVertexShader->inDesc[i].Size;
	}
	unsigned char *vertexBuffer = new unsigned char[Size];
	////获取一个片元的索引
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		unsigned char *o0 = Vertex(i, vertexBuffer);
		unsigned char *o1 = Vertex(i + 1, vertexBuffer);
		unsigned char *o2 = Vertex(i + 2, vertexBuffer);

		std::unordered_map<std::string, glm::vec4> out0, out1, out2;
		ParseShaderOutput(o0, out0);
		ParseShaderOutput(o1, out1);
		ParseShaderOutput(o2, out2);
		if (out0.find("SV_Position") == out0.end()) return;
		//视口变换
		ViewportTransform(out0["SV_Position"], out1["SV_Position"], out2["SV_Position"]);
		Triangle(out0, out1, out2);
		delete o0;
		delete o1;
		delete o2;
	}
	//delete vertexBuffer;
}

glm::vec3 DeviceContext::Barycentric(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 P)
{
	glm::vec3 s[2];
	for (int i = 1; i >= 0; i--)
	{
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	glm::vec3 u = glm::cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2)
		return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return glm::vec3(-1, 1, 1);
}

glm::vec3 BarycentricLerp(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &w)
{
	return w[0] * v0 + w[1] * v1 + w[2] * v2;
}

void DeviceContext::Triangle(std::unordered_map<std::string, glm::vec4> &o0, std::unordered_map<std::string, glm::vec4> &o1, std::unordered_map<std::string, glm::vec4> &o2)
{
	glm::vec3 t0 = glm::i32vec3(o0["SV_Position"].x, o0["SV_Position"].y, o0["SV_Position"].z);
	glm::vec3 t1 = glm::i32vec3(o1["SV_Position"].x, o1["SV_Position"].y, o1["SV_Position"].z);
	glm::vec3 t2 = glm::i32vec3(o2["SV_Position"].x, o2["SV_Position"].y, o2["SV_Position"].z);

	if (t0.y == t1.y && t0.y == t2.y) return; // I dont care about degenerate triangles 
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	//排序顶点时不要忘记排序其他属性
	if (t0.y > t1.y)
	{
		std::swap(t0, t1);
		std::swap(o0, o1);
	}
	if (t0.y > t2.y)
	{
		std::swap(t0, t2);
		std::swap(o0, o2);
	}
	if (t1.y > t2.y)
	{
		std::swap(t1, t2);
		std::swap(o1, o2);
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
		unsigned char *pixelInput = new unsigned char[100];
		for (int j = A.x; j <= B.x; j++) 
		{
			glm::vec2 P(j, t0.y + i);
			//执行裁剪
			if ((j < 0 && B.x < 0) || P.y < 0 || j >= pViewports->Width || P.y >= pViewports->Height) break;
			if (j < 0) continue;
			glm::vec3 bcScreen = Barycentric(t0, t1, t2, P);
			//插值深度
			float depth = bcScreen[0] * t0.z + bcScreen[1] * t1.z + bcScreen[2] * t2.z;
			if (bcScreen[0] < 0 || bcScreen[1] < 0 || bcScreen[2] < 0 || pDepthStencilView->GetDepth(j, t0.y + i) - 0.0001f < depth)
			{
				continue;
			}
			if (o0.find("SV_TEXCOORD0") != o0.end())
			{
				QuadFragments quadFragment;
				quadFragment.m_fragments[0] = BarycentricLerp(o0["SV_TEXCOORD0"], o1["SV_TEXCOORD0"], o2["SV_TEXCOORD0"], Barycentric(t0, t1, t2, glm::vec3(P.x, P.y, 1.0)));
				quadFragment.m_fragments[1] = BarycentricLerp(o0["SV_TEXCOORD0"], o1["SV_TEXCOORD0"], o2["SV_TEXCOORD0"], Barycentric(t0, t1, t2, glm::vec3(P.x + 1, P.y, 1.0)));
				quadFragment.m_fragments[2] = BarycentricLerp(o0["SV_TEXCOORD0"], o1["SV_TEXCOORD0"], o2["SV_TEXCOORD0"], Barycentric(t0, t1, t2, glm::vec3(P.x, P.y + 1, 1.0)));
				quadFragment.m_fragments[3] = BarycentricLerp(o0["SV_TEXCOORD0"], o1["SV_TEXCOORD0"], o2["SV_TEXCOORD0"], Barycentric(t0, t1, t2, glm::vec3(P.x + 1, P.y + 1, 1.0)));
				glm::vec2 ddx(quadFragment.dUdx(), quadFragment.dUdy());
				glm::vec2 ddy(quadFragment.dVdx(), quadFragment.dVdy());
				memcpy(&pPixelShader->ddx, &ddx, sizeof(glm::vec2));
				memcpy(&pPixelShader->ddy, &ddy, sizeof(glm::vec2));
			}
			//插值其他信息
			for (auto iter : pPixelShader->inDesc)
			{
				glm::vec4 attribute = o0[iter.Name] * bcScreen[0] + o1[iter.Name] * bcScreen[1] + o2[iter.Name] * bcScreen[2];
				memcpy(pixelInput + iter.Offset, &attribute, iter.Size);
			}

			glm::vec4 colors;
			bool discard = pPixelShader->fragment(pixelInput, colors);
			pRenderTargetView->SetPixel(P.x, P.y, colors[0], colors[1], colors[2], colors[3]);
			pDepthStencilView->SetDepth(j, t0.y + i, depth);
		}
		delete pixelInput;
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
			output[pVertexShader->outDesc[i].Name] = glm::vec4(*start, *(start + 1), 0, 1);
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
			output[pVertexShader->outDesc[i].Name] = glm::vec4(*start, *(start + 1), *(start + 2), *(start + 3)) / *(start + 3);
			start += 4;
		}
		break;
		}
		offset += pVertexShader->outDesc[i].Size;
	}
}

void DeviceContext::viewport(int x, int y, int w, int h)
{
	Viewport = glm::mat4x4(1.0);
	Viewport[3][0] = x + w / 2.f;
	Viewport[3][1] = x + h / 2.f;
	Viewport[3][2] = 10000.f / 2.f;

	Viewport[0][0] = w / 2;
	Viewport[1][1] = h / 2;
	Viewport[2][2] = 10000.f / 2.f;
}

void DeviceContext::ViewportTransform(glm::vec4 &t0, glm::vec4 &t1, glm::vec4 &t2)
{
	t0 = Viewport * t0;
	t1 = Viewport * t1;
	t2 = Viewport * t2;
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
