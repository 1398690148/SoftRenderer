#include <algorithm>
#include "DeviceContext.h"
#include "IVertexShader.h"
#include "IPixelShader.h"
#include "InputLayout.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

DeviceContext::DeviceContext()
{
	SV_PositionIndex = -1;
}

DeviceContext::~DeviceContext()
{
}

void DeviceContext::ClearRenderTargetView(RenderTargetView *RenderTargetView, const float ColorRGBA[4])
{
	RenderTargetView->ClearBuffer(ColorRGBA);
}

void DeviceContext::IASetVertexBuffers(IBuffer * buf, const unsigned int *pStrides, const unsigned int *pOffsets)
{
	pVertexBuffer = buf;
}

void DeviceContext::IASetIndexBuffer(IBuffer * buf, unsigned int Offset)
{
	pIndexBuffer = buf;
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

void DeviceContext::RSSetViewports(unsigned int NumViewports, const VIEWPORT * Viewports)
{
	pViewports = Viewports;
	viewport(0, 0, Viewports->Width, Viewports->Height);
}

void DeviceContext::OMSetRenderTargets(RenderTargetView **RenderTargetView, DepthStencilView **DepthStencilView)
{
	pRenderTargetView = *RenderTargetView;
	pDepthStencilView = *DepthStencilView;
}

void DeviceContext::DrawIndex()
{
	ParseVertexBuffer();
	//pVertexShader->buffer = new unsigned char[sizeof(Viewport)];
	//memcpy(pVertexShader->buffer, &Viewport, sizeof(Viewport));
	////获取一个片元的索引
	for (unsigned int offset = 0; offset < pIndexBuffer->GetByteWidth();)
	{
		unsigned int *indexBuffer = (unsigned int *)pIndexBuffer->GetBuffer(offset);
		offset += pIndexBuffer->GetStructureByteStride() * 3;

		int Size = 0;
		for (int i = 0; i < pVertexShader->inDesc.size(); i++)
		{
			Size += pVertexShader->inDesc[i].Size;
		}
		unsigned char *vertexBuffer = new unsigned char[Size];

		for (int i = 0; i < pVertexShader->inDesc.size(); i++)
		{
			const Attribute &attr = m_Data[pVertexShader->inDesc[i].Name];
			memcpy(vertexBuffer + attr.Offset, &attr.data[indexBuffer[0]], attr.Size);
		}
		unsigned char *o1 = pVertexShader->vertex(vertexBuffer);

		for (int i = 0; i < pVertexShader->inDesc.size(); i++)
		{
			const Attribute &attr = m_Data[pVertexShader->inDesc[i].Name];
			memcpy(vertexBuffer + attr.Offset, &attr.data[indexBuffer[1]], attr.Size);
		}
		unsigned char *o2 = pVertexShader->vertex(vertexBuffer);

		for (int i = 0; i < pVertexShader->inDesc.size(); i++)
		{
			const Attribute &attr = m_Data[pVertexShader->inDesc[i].Name];
			memcpy(vertexBuffer + attr.Offset, &attr.data[indexBuffer[2]], attr.Size);
		}
		unsigned char *o3 = pVertexShader->vertex(vertexBuffer);

		triangle(o1, o2, o3);
		delete vertexBuffer;
	}
}

glm::vec3 DeviceContext::barycentric(glm::i32vec2 A, glm::i32vec2 B, glm::i32vec2 C, glm::i32vec2 P)
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

void DeviceContext::triangle(unsigned char *o1, unsigned char *o2, unsigned char *o3)
{
	std::vector<glm::vec4> shaderOutput0;
	std::vector<glm::vec4> shaderOutput1;
	std::vector<glm::vec4> shaderOutput2;
	//解析顶点着色器输出
	ParseShaderOutput(o1, shaderOutput0);
	ParseShaderOutput(o2, shaderOutput1);
	ParseShaderOutput(o3, shaderOutput2);
	if (SV_PositionIndex == -1) return;

	shaderOutput0[SV_PositionIndex] = shaderOutput0[SV_PositionIndex] / shaderOutput0[SV_PositionIndex].w;
	shaderOutput1[SV_PositionIndex] = shaderOutput1[SV_PositionIndex] / shaderOutput1[SV_PositionIndex].w;
	shaderOutput2[SV_PositionIndex] = shaderOutput2[SV_PositionIndex] / shaderOutput2[SV_PositionIndex].w;

	shaderOutput0[SV_PositionIndex] = shaderOutput0[SV_PositionIndex] * Viewport;
	shaderOutput1[SV_PositionIndex] = shaderOutput1[SV_PositionIndex] * Viewport;
	shaderOutput2[SV_PositionIndex] = shaderOutput2[SV_PositionIndex] * Viewport;
	glm::vec3 t0 = glm::i32vec3(shaderOutput0[SV_PositionIndex].x, shaderOutput0[SV_PositionIndex].y, shaderOutput0[SV_PositionIndex].z);
	glm::vec3 t1 = glm::i32vec3(shaderOutput1[SV_PositionIndex].x, shaderOutput1[SV_PositionIndex].y, shaderOutput1[SV_PositionIndex].z);
	glm::vec3 t2 = glm::i32vec3(shaderOutput2[SV_PositionIndex].x, shaderOutput2[SV_PositionIndex].y, shaderOutput2[SV_PositionIndex].z);

	if (t0.y == t1.y && t0.y == t2.y) return; // I dont care about degenerate triangles 
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	//排序顶点时不要忘记排序其他属性
	if (t0.y > t1.y)
	{
		std::swap(t0, t1);
		std::swap(shaderOutput0, shaderOutput1);
	}
	if (t0.y > t2.y)
	{
		std::swap(t0, t2);
		std::swap(shaderOutput0, shaderOutput2);
	}
	if (t1.y > t2.y)
	{
		std::swap(t1, t2);
		std::swap(shaderOutput1, shaderOutput2);
	}
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
		glm::vec3 A = t0 + (t2 - t0) * alpha;
		glm::vec3 B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		unsigned char *pixelInput = new unsigned char[100];
		for (int j = A.x; j <= B.x; j++) {
			glm::i32vec2 P(j, t0.y + i);
			glm::vec3 bcScreen = barycentric(t0, t1, t2, P);
			//插值深度
			unsigned char depth = std::max(0, std::min(255, (int)(bcScreen[0] * t0.z + bcScreen[1] * t1.z + bcScreen[2] * t2.z)));
			if (bcScreen[0] < 0 || bcScreen[1] < 0 || bcScreen[2] < 0 || pDepthStencilView->GetDepth(j, t0.y + i) > depth)
			{
				continue;
			}
			//插值其他信息
			for (int k = 0; k < pVertexShader->outDesc.size(); k++)
			{
				glm::vec4 attribute = shaderOutput0[k] * bcScreen[0] + shaderOutput1[k] * bcScreen[1] + shaderOutput2[k] * bcScreen[2];
				memcpy(pixelInput + pVertexShader->outDesc[k].Offset, &attribute, pVertexShader->outDesc[k].Size);
			}
			glm::vec4 colors;
			glm::vec4 Q(P.x, P.y, depth, 1.0);
			bool discard = pPixelShader->fragment(pixelInput, colors);
			pRenderTargetView->SetPixel(Q.x, Q.y, colors[0], colors[1], colors[2], colors[3]);
			pDepthStencilView->SetDepth(j, t0.y + i, depth);
		}
		delete pixelInput;
	}
}

void DeviceContext::ParseVertexBuffer()
{
	int bufferSize = pVertexBuffer->GetByteWidth() / pVertexBuffer->GetStructureByteStride();
	int outOffset = 0;
	for (int i = 0; i < pInputLayout->getSize(); i++)
	{
		const INPUT_ELEMENT_DESC *desc = pInputLayout->getData(i);
		Attribute attr;
		if (desc->SemanticName == "Position")
		{
			int num = pInputLayout->getData(i)->Size / pInputLayout->getData(i)->typeSize;
			attr.Size = pInputLayout->getData(i)->Size;
			attr.Offset = outOffset;
			int inOffset = pInputLayout->getData(i)->Offset;
			for (int j = 0; j < bufferSize; j++)
			{
				glm::vec4 position;
				position[0] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset));
				position[1] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + desc->typeSize));
				if (num > 2)
				{
					position[2] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 2 * desc->typeSize));
					if (num > 3)
					{
						position[3] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 3 * desc->typeSize));
					}
				}
				attr.data.push_back(position);
			}
		}
		//else if (desc->SemanticName == "Normal")
		//{
		//	int num = pInputLayout->getData(i)->Size / pInputLayout->getData(i)->typeSize;
		//	normalElemSize = pInputLayout->getData(i)->typeSize;
		//	normalSize = pInputLayout->getData(i)->Size;
		//	normalOffset = pInputLayout->getData(i)->Offset;
		//	for (int j = 0; j < bufferSize; j++)
		//	{
		//		Vec4f Normal;
		//		Normal[0] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + normalOffset));
		//		Normal[1] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + normalOffset + desc->typeSize));
		//		if (num > 2)
		//		{
		//			Normal[2] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + normalOffset + 2 * desc->typeSize));
		//			if (num > 3)
		//			{
		//				Normal[3] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + normalOffset + 3 * desc->typeSize));
		//			}
		//		}
		//		normals.push_back(Normal);
		//	}
		//}
		else if (desc->SemanticName == "Color")
		{
			int num = pInputLayout->getData(i)->Size / pInputLayout->getData(i)->typeSize;
			attr.Size = pInputLayout->getData(i)->Size / pInputLayout->getData(i)->typeSize * 4;
			attr.Offset = outOffset;
			int inOffset = pInputLayout->getData(i)->Offset;
			for (int j = 0; j < bufferSize; j++)
			{
				glm::vec4 Color;
				if (desc->typeSize == 1)
				{
					Color[0] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset));
					Color[1] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + desc->typeSize));
					if (num > 2)
					{
						Color[2] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 2 * desc->typeSize));
						if (num > 3)
						{
							Color[3] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 3 * desc->typeSize));
						}
					}
				}
				else if (desc->typeSize == 2)
				{
					Color[0] = *((unsigned short *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset));
					Color[1] = *((unsigned short *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + desc->typeSize));
					if (num > 2)
					{
						Color[2] = *((unsigned short *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 2 * desc->typeSize));
						if (num > 3)
						{
							Color[3] = *((unsigned short *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 3 * desc->typeSize));
						}
					}
				}
				else if (desc->typeSize == 4)
				{
					Color[0] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset));
					Color[1] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + desc->typeSize));
					if (num > 2)
					{
						Color[2] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 2 * desc->typeSize));
						if (num > 3)
						{
							Color[3] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 3 * desc->typeSize));
						}
					}
				}
				attr.data.push_back(Color);
			}
		}
		else if (desc->SemanticName == "UV")
		{
			int num = pInputLayout->getData(i)->Size / pInputLayout->getData(i)->typeSize;
			attr.Size = pInputLayout->getData(i)->Size;
			attr.Offset = outOffset;
			int inOffset = pInputLayout->getData(i)->Offset;
			for (int j = 0; j < bufferSize; j++)
			{
				glm::vec4 UV;
				UV[0] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset));
				UV[1] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + desc->typeSize));
				if (num > 2)
				{
					UV[2] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * j + inOffset + 2 * desc->typeSize));
				}
				attr.data.push_back(UV);
			}
		}
		m_Data[desc->SemanticName] = attr;
		outOffset += 16;
	}
}

void DeviceContext::ParseShaderOutput(unsigned char * buffer, std::vector<glm::vec4> &output)
{
	int offset = 0;
	for (int i = 0; i < pVertexShader->outDesc.size(); i++)
	{
		if (pVertexShader->outDesc[i].Name == "SV_Position")
		{
			SV_PositionIndex = i;
		}
		switch (pVertexShader->outDesc[i].Size / pVertexShader->outDesc[i].typeSize)
		{
		case 2:
		{
			output.push_back(glm::vec4(*(float *)(buffer + offset), *(float *)(buffer + offset + pVertexShader->outDesc[i].typeSize), 0, 0));
		}
		break;
		case 3:
		{
			output.push_back(glm::vec4(*(float *)(buffer + offset), *(float *)(buffer + offset + pVertexShader->outDesc[i].typeSize), *(float *)(buffer + offset + 2 * pVertexShader->outDesc[i].typeSize), 0));
		}
		break;
		case 4:
		{
			output.push_back(glm::vec4(*(float *)(buffer + offset), *(float *)(buffer + offset + pVertexShader->outDesc[i].typeSize),
				*(float *)(buffer + offset +2 *  pVertexShader->outDesc[i].typeSize), *(float *)(buffer + offset + 3 * pVertexShader->outDesc[i].typeSize)));
		}
		break;
		}
		offset += pVertexShader->outDesc[i].Size;
	}
}

void DeviceContext::viewport(int x, int y, int w, int h)
{
	Viewport = glm::mat4x4(1.0);
	Viewport[0][3] = x + w / 2.f;
	Viewport[1][3] = x + h / 2.f;
	Viewport[2][3] = 255.f / 2.f;

	Viewport[0][0] = w / 2;
	Viewport[1][1] = h / 2;
	Viewport[2][2] = 255.f / 2.f;
}