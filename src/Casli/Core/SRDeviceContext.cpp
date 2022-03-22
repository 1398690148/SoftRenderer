#include <algorithm>
#include "SRDeviceContext.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include <iostream>

SRDeviceContext::SRDeviceContext(void *gFbo, int width, int height)
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
	pBlendState = new SRBlendState(&desc);
	pFrontBuffer = new SRRenderTargetView(width, height, 4, gFbo);
	msaaBuffer.resize(width * height);
}

SRDeviceContext::~SRDeviceContext()
{
}

void SRDeviceContext::ClearRenderTargetView(SRRenderTargetView *RenderTargetView, const glm::vec4 &ColorRGBA)
{
	pBackBuffer->ClearBuffer(ColorRGBA);
	ClearMSAABuffer();
}

void SRDeviceContext::ClearDepthStencilView(SRDepthStencilView *DepthStencilView)
{
	//DepthStencilView->ClearBuffer(FLT_MAX);
}

void SRDeviceContext::IASetVertexBuffers(SRIBuffer *buf, const unsigned int *pStrides, const unsigned int *pOffsets)
{
	pVertexBuffer = buf;
}

void SRDeviceContext::IASetIndexBuffer(SRIBuffer * buf, unsigned int Offset)
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

void SRDeviceContext::VSSetShader(SRIVertexShader *shader)
{
	pVertexShader = shader;
	for (int i = 0; i < shader->outDesc.size(); i++)
	{
		vertexOutMapTable[shader->outDesc[i].Name] = i;
	}
	posIdx = vertexOutMapTable["SV_Position"];
}

void SRDeviceContext::PSSetShader(SRIPixelShader *shader)
{
	pPixelShader = shader;
}

void SRDeviceContext::PSSetShaderResources(int slot, SRTexture2D **texture)
{
	if (slot >= 16) return;
	pPixelShader->textures[slot] = *texture;
}

void SRDeviceContext::PSSetSamplerState(int slot, SRSamplerState **sampler)
{
	if (slot >= 4) return;
	pPixelShader->samplers[slot] = *sampler;
}

void SRDeviceContext::OMSetBlendState(SRBlendState **blendState, const float *BlendFactor, unsigned int SampleMask)
{
	pBlendState = *blendState;
	if (BlendFactor)
		memcpy(pBlendFactor, BlendFactor, sizeof(float));
}

void SRDeviceContext::OMGetBlendState(SRBlendState **ppBlendState, float *BlendFactor, unsigned int *SampleMask)
{
	*ppBlendState = pBlendState;
	BlendFactor = pBlendFactor;
	*SampleMask = pSampleMask;
}

void SRDeviceContext::SetRenderState(ShaderState state)
{
	pShaderState = state;
}

void SRDeviceContext::SwapBuffer()
{
	Resolve();
	std::swap(pFrontBuffer, pBackBuffer);
}

void SRDeviceContext::VSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, SRIBuffer *constantBuffer)
{
	pVertexConstantBuffer = constantBuffer;
}

void SRDeviceContext::PSSetConstantBuffers(unsigned int StartOffset, unsigned int NumBuffers, SRIBuffer *constantBuffer)
{
	pPixelConstantBuffer = constantBuffer;
}

void SRDeviceContext::IASetInputLayout(SRInputLayout *InputLayout)
{
	pInputLayout = InputLayout;
}

void SRDeviceContext::IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology)
{
	pTopology = topology;
}

void SRDeviceContext::RSSetViewports(unsigned int NumViewports, const VIEWPORT *Viewports)
{
	pViewports = new VIEWPORT();
	memcpy(pViewports, Viewports, sizeof(VIEWPORT));
	Viewport = glm::mat4x4(1.0);
	Viewport[3][0] = (pViewports->Width + pViewports->TopLeftX) >> 1;
	Viewport[3][1] = (pViewports->Height - pViewports->TopLeftY) >> 1;

	Viewport[0][0] = pViewports->Width >> 1;
	Viewport[1][1] = pViewports->Height >> 1;
}

void SRDeviceContext::OMSetRenderTargets(SRRenderTargetView **RenderTargetView, SRDepthStencilView **DepthStencilView)
{
	pBackBuffer = *RenderTargetView;
	pDepthStencilView = *DepthStencilView;
}

void SRDeviceContext::GenerateMips(SRTexture2D *texture)
{
	texture->GenerateMips();
}

std::vector<std::vector<glm::vec4>> SRDeviceContext::ClipSutherlandHodgemanAux(const std::vector<std::vector<glm::vec4>> &polygon, 
	const int axis, const int side)
{
	std::vector<std::vector<glm::vec4>> insidePolygon;

	int numVerts = polygon.size();
	for (int i = 0; i < numVerts; ++i)
	{
		const auto &begVert = polygon[(i - 1 + numVerts) % numVerts];
		const auto &endVert = polygon[i];
		char begIsInside = ((side * (begVert[posIdx][axis]) <= begVert[posIdx].w) ? 1 : -1);
		char endIsInside = ((side * (endVert[posIdx][axis]) <= endVert[posIdx].w) ? 1 : -1);
		//One of them is outside
		if (begIsInside * endIsInside < 0)
		{
			// t = (w1 - y1)/((w1-y1)-(w2-y2))
			float t = (begVert[posIdx].w - side * begVert[posIdx][axis])
				/ ((begVert[posIdx].w - side * begVert[posIdx][axis]) - (endVert[posIdx].w - side * endVert[posIdx][axis]));
			std::vector<glm::vec4> intersectedVert;
			for (int j = 0; j < begVert.size(); j++)
			{
				intersectedVert.push_back(Utils::lerp(begVert[j], endVert[j], t));
			}
			insidePolygon.push_back(intersectedVert);
		}
		//If current vertices is inside
		if (endIsInside > 0)
		{
			insidePolygon.push_back(endVert);
		}
	}
	return insidePolygon;
}


std::vector<std::vector<glm::vec4>> SRDeviceContext::ClipSutherlandHodgeman(
	const std::vector<glm::vec4> &vertex0,
	const std::vector<glm::vec4> &vertex1,
	const std::vector<glm::vec4> &vertex2,
	const float Near, const float Far)
{
	glm::vec4 v0 = vertex0[posIdx];
	glm::vec4 v1 = vertex1[posIdx];
	glm::vec4 v2 = vertex2[posIdx];
	auto isPointInsideInClipingFrustum = [](const glm::vec4 &p, const float &Near, const float &Far) -> bool
	{
		return (p.x <= p.w && p.x >= -p.w) && (p.y <= p.w && p.y >= -p.w)
			&& (p.z <= p.w && p.z >= -p.w) && (p.w <= Far && p.w >= Near);
	};

	//Totally inside
	if (isPointInsideInClipingFrustum(v0, Near, Far) &&
		isPointInsideInClipingFrustum(v1, Near, Far) &&
		isPointInsideInClipingFrustum(v2, Near, Far))
	{
		return { vertex0,vertex1,vertex2 };
	}
	//Totally outside
	if (v0.w < Near && v1.w < Near && v2.w < Near)
		return{};
	if (v0.w > Far && v1.w > Far && v2.w > Far)
		return{};
	if (v0.x > v0.w && v1.x > v1.w && v2.x > v2.w)
		return{};
	if (v0.x < -v0.w && v1.x < -v1.w && v2.x < -v2.w)
		return{};
	if (v0.y > v0.w && v1.y > v1.w && v2.y > v2.w)
		return{};
	if (v0.y < -v0.w && v1.y < -v1.w && v2.y < -v2.w)
		return{};
	if (v0.z > v0.w && v1.z > v1.w && v2.z > v2.w)
		return{};
	if (v0.z < -v0.w && v1.z < -v1.w && v2.z < -v2.w)
		return{};
	std::vector<std::vector<glm::vec4>> insideVertices;
	std::vector<std::vector<glm::vec4>> tmp = { vertex0, vertex1, vertex2 };
	enum Axis { X = 0, Y = 1, Z = 2 };

	//w=x plane & w=-x plane
	insideVertices = ClipSutherlandHodgemanAux(tmp, Axis::X, +1);
	tmp = insideVertices;
	insideVertices = ClipSutherlandHodgemanAux(tmp, Axis::X, -1);
	tmp = insideVertices;

	//w=y plane & w=-y plane
	insideVertices = ClipSutherlandHodgemanAux(tmp, Axis::Y, +1);
	tmp = insideVertices;
	insideVertices = ClipSutherlandHodgemanAux(tmp, Axis::Y, -1);
	tmp = insideVertices;

	//w=z plane & w=-z plane
	insideVertices = ClipSutherlandHodgemanAux(tmp, Axis::Z, +1);
	tmp = insideVertices;
	insideVertices = ClipSutherlandHodgemanAux(tmp, Axis::Z, -1);
	tmp = insideVertices;

	//w=1e-5 plane
	insideVertices = {};
	int numVerts = tmp.size();
	constexpr float wClippingPlane = 1e-5;
	for (int i = 0; i < numVerts; ++i)
	{
		const auto &begVert = tmp[(i - 1 + numVerts) % numVerts];
		const auto &endVert = tmp[i];
		float begIsInside = (begVert[posIdx].w < wClippingPlane) ? -1 : 1;
		float endIsInside = (endVert[posIdx].w < wClippingPlane) ? -1 : 1;
		//One of them is outside
		if (begIsInside * endIsInside < 0)
		{
			// t = (w_clipping_plane-w1)/((w1-w2)
			float t = (wClippingPlane - begVert[posIdx].w) / (begVert[posIdx].w - endVert[posIdx].w);
			std::vector<glm::vec4> intersectedVert;
			for (int j = 0; j < begVert.size(); j++)
			{
				intersectedVert.push_back(Utils::lerp(begVert[j], endVert[j], t));
			}
			insideVertices.push_back(intersectedVert);
		}
		//If current vertices is inside
		if (endIsInside > 0)
		{
			insideVertices.push_back(endVert);
		}
	}
	return insideVertices;
}

void SRDeviceContext::DrawIndex()
{
	ParseVertexBuffer();
	BindConstanBuffer();
	tbb::parallel_for(0, (int)indices.size(), [&](size_t i)
	{
		unsigned char *buffer = new unsigned char[160];
		unsigned char *o0 = Vertex(indices[i][0], buffer);
		unsigned char *o1 = Vertex(indices[i][1], buffer);
		unsigned char *o2 = Vertex(indices[i][2], buffer);

		std::vector<glm::vec4> vertex[3];
		for (int i = 0; i < 3; i++)
		{
			vertex[i].resize(vertexOutMapTable.size());
		}
		ParseShaderOutput(o0, vertex[0]);
		ParseShaderOutput(o1, vertex[1]);
		ParseShaderOutput(o2, vertex[2]);

		std::vector<std::vector<glm::vec4>> clipped_vertices = ClipSutherlandHodgeman(vertex[0], vertex[1], vertex[2], 0.1, 100);
		int len = (int)clipped_vertices.size() - 2;
		for (int i = 0; i < len; i++)
		{
			vertex[0] = clipped_vertices[0];
			vertex[1] = clipped_vertices[i + 1];
			vertex[2] = clipped_vertices[i + 2];
			prePerspCorrection(vertex);
			//视口变换
			ViewportTransform(vertex);
			Triangle(vertex);
		}

		delete o0;
		delete o1;
		delete o2;
		delete buffer;
	});
}

bool SRDeviceContext::shouldCulled(const glm::ivec2 &v0, const glm::ivec2 &v1, const glm::ivec2 &v2)
{
	if (pShaderState.m_CullFaceMode == CullFaceMode::CULL_DISABLE) return false;
	//Back face culling in screen space
	auto e1 = v1 - v0;
	auto e2 = v2 - v0;
	int orient = e1.x * e2.y - e1.y * e2.x;
	return pShaderState.m_CullFaceMode == CullFaceMode::CULL_FRONT ? orient > 0 : orient < 0;
}

void SRDeviceContext::Triangle(std::vector<glm::vec4> vertex[3])
{
	std::vector<glm::vec3> points = { vertex[0][posIdx], vertex[1][posIdx], vertex[2][posIdx] };
	if (shouldCulled(points[0], points[1], points[2])) return;

	glm::ivec2 bboxmin(pViewports->Width - 1, pViewports->Height - 1);
	glm::ivec2 bboxmax(0, 0);
	glm::ivec2 clamp(pViewports->Width - 1, pViewports->Height - 1);

	for (int i = 0; i < 3; i++) 
	{
		bboxmin.x = max(0, min(bboxmin.x, points[i].x));
		bboxmin.y = max(0, min(bboxmin.y, points[i].y));
		bboxmax.x = min(clamp.x, max(bboxmax.x, points[i].x));
		bboxmax.y = min(clamp.y, max(bboxmax.y, points[i].y));
	}
	tbb::parallel_for(bboxmin.y, bboxmax.y + 1, 1, [&](size_t y)
	{
		tbb::parallel_for(bboxmin.x, bboxmax.x + 1, 1, [&](size_t x)
		{
			glm::ivec2 P(x, y);
			//插值深度
			glm::vec3 bcScreen = Utils::Barycentric(points[0], points[1], points[2], glm::vec2(P.x + 0.5, P.y + 0.5));

			MSAAData data = CoverageCalc(x, y, points);
			int coverage = 0;
			auto &fragCache = msaaBuffer[P.x + P.y * (pViewports->Width - 1)];
			for (int i = 0; i < 4; i++)
			{
				if (data.coverage[i] && (pBlendState->blendDesc.RenderTarget[0].BlendEnable || data.depth[i] < fragCache.depth[i]))
				{
					coverage++;
				}
			}
			if (coverage == 0) return;
			if (vertexOutMapTable.count("SV_TEXCOORD0"))
				DDXDDY(vertex, points[0], points[1], points[2], glm::vec2(P.x + 0.5, P.y + 0.5));
			glm::vec4 color(0, 0, 0, 255);
			unsigned char *buffer = Interpolation(vertex, bcScreen);
			bool discard = pPixelShader->fragment(buffer, color);
			delete buffer;
			//Alpha Test
			if (pBlendState->blendDesc.RenderTarget[0].BlendEnable && discard) return;
			for (int i = 0; i < 4; i++)
			{
				if (!data.coverage[i]) continue;
				//Alpha Blend
				if (pBlendState->blendDesc.RenderTarget[0].BlendEnable)
				{
					fragCache.coverage[i] = 1;
					AlphaBlend(color, fragCache.color[i]);
					fragCache.color[i] = color;
					fragCache.depth[i] = data.depth[i];
				}
				else if (data.depth[i] < fragCache.depth[i])
				{
					fragCache.coverage[i] = 1;
					fragCache.color[i] = color;
					fragCache.depth[i] = data.depth[i];
				}
			}
			//Early-Z
			//if (bcScreen[0] < 0 || bcScreen[1] < 0 || bcScreen[2] < 0 || 
			//	(!pBlendState->blendDesc.RenderTarget[0].BlendEnable && pDepthStencilView->GetDepth(P.x, P.y) - 0.0001f < depth))
			//	return;
			//{
			//	if (vertexOutMapTable.count("SV_TEXCOORD0"))
			//		DDXDDY(vertex, points[0], points[1], points[2], P);
			//	glm::vec4 color(0, 0, 0, 255);
			//	unsigned char *buffer = Interpolation(vertex, bcScreen);
			//	bool discard = pPixelShader->fragment(buffer, color);
			//	delete buffer;
			//	if (pBlendState->blendDesc.RenderTarget[0].BlendEnable)
			//	{
			//		if (discard) return;
			//		AlphaBlend(P.x, P.y, color);
			//		pBackBuffer->SetPixel(P.x, P.y, color[0], color[1], color[2], color[3]);
			//		return;
			//	}
			//	if (pDepthStencilView->GetDepth(P.x, P.y) < depth)
			//	{
			//		return;
			//	}
			//	pBackBuffer->SetPixel(P.x, P.y, color[0], color[1], color[2], color[3]);
			//	pDepthStencilView->SetDepth(P.x, P.y, depth);
			// }
		});
	});
}

void SRDeviceContext::ParseVertexBuffer()
{
	int bufferSize = pVertexBuffer->GetByteWidth() / pVertexBuffer->GetStructureByteStride();
	for (int i = 0; i < pInputLayout->getSize(); i++)
	{
		const INPUT_ELEMENT_DESC *desc = pInputLayout->getData(i);
		std::vector<glm::vec4> attr;
		attr.resize(bufferSize);
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
			attr[j] = v;
		}
		m_Data[desc->SemanticName] = attr;
	}
}

void SRDeviceContext::ParseShaderOutput(unsigned char *buffer, std::vector<glm::vec4> &output)
{
	int offset = 0;
	for (int i = 0; i < pVertexShader->outDesc.size(); i++)
	{
		float *start = (float *)(buffer + offset);
		switch (pVertexShader->outDesc[i].Size / pVertexShader->outDesc[i].typeSize)
		{
		case 2:
		{
			output[i] = glm::vec4((*start), *(start + 1), 1, 1);
		}
		break;
		case 3:
		{
			output[i] = glm::vec4(*start, *(start + 1), *(start + 2), 1);
		}
		break;
		case 4:
		{
			output[i] = glm::vec4(*start, *(start + 1), *(start + 2), *(start + 3));
		}
		break;
		}
		offset += pVertexShader->outDesc[i].Size;
	}
}

void SRDeviceContext::ViewportTransform(std::vector<glm::vec4> vertex[3])
{
	vertex[0][posIdx] = Viewport * vertex[0][posIdx] + glm::vec4(0.5f);
	vertex[1][posIdx] = Viewport * vertex[1][posIdx] + glm::vec4(0.5f);
	vertex[2][posIdx] = Viewport * vertex[2][posIdx] + glm::vec4(0.5f);
}

unsigned char * SRDeviceContext::Vertex(int idx, unsigned char *vertexBuffer)
{
	for (int i = 0; i < pVertexShader->inDesc.size(); i++)
	{
		const std::vector<glm::vec4> &attr = m_Data[pVertexShader->inDesc[i].Name];
		memcpy(vertexBuffer + pVertexShader->inDesc[i].Offset, &attr[idx], pVertexShader->inDesc[i].Size);
	}
	return pVertexShader->vertex(vertexBuffer);
}

void SRDeviceContext::DDXDDY(std::vector<glm::vec4> vertex[3], glm::vec3 &t0, glm::vec3 &t1, glm::vec3 &t2, glm::vec2 &P)
{
	unsigned int texCoordIdx = vertexOutMapTable["SV_TEXCOORD0"];
	QuadFragments quadFragment;
	glm::vec3 texCoord0 = vertex[0][texCoordIdx];
	glm::vec3 texCoord1 = vertex[1][texCoordIdx];
	glm::vec3 texCoord2 = vertex[2][texCoordIdx];
	quadFragment.m_fragments[0] = Utils::BarycentricLerp(texCoord0, texCoord1, texCoord2, Utils::Barycentric(t0, t1, t2, glm::vec3(P.x, P.y, 1.0)));
	quadFragment.m_fragments[0] /= quadFragment.m_fragments[0].z;
	quadFragment.m_fragments[1] = Utils::BarycentricLerp(texCoord0, texCoord1, texCoord2, Utils::Barycentric(t0, t1, t2, glm::vec3(P.x + 1, P.y, 1.0)));
	quadFragment.m_fragments[1] /= quadFragment.m_fragments[1].z;
	quadFragment.m_fragments[2] = Utils::BarycentricLerp(texCoord0, texCoord1, texCoord2, Utils::Barycentric(t0, t1, t2, glm::vec3(P.x, P.y + 1, 1.0)));
	quadFragment.m_fragments[2] /= quadFragment.m_fragments[2].z;
	quadFragment.m_fragments[3] = Utils::BarycentricLerp(texCoord0, texCoord1, texCoord2, Utils::Barycentric(t0, t1, t2, glm::vec3(P.x + 1, P.y + 1, 1.0)));
	quadFragment.m_fragments[3] /= quadFragment.m_fragments[3].z;
	glm::vec2 ddx(quadFragment.dUdx(), quadFragment.dUdy());
	glm::vec2 ddy(quadFragment.dVdx(), quadFragment.dVdy());
	memcpy(&pPixelShader->dFdx.local(), &ddx, sizeof(glm::vec2));
	memcpy(&pPixelShader->dFdy.local(), &ddy, sizeof(glm::vec2));
}

void SRDeviceContext::prePerspCorrection(std::vector<glm::vec4> output[3])
{
	unsigned int normalIdx = vertexOutMapTable["SV_Normal"];
	unsigned int texCoordIdx = vertexOutMapTable["SV_TEXCOORD0"];
	for (int i = 0; i < 3; i++)
	{
		float t = 1.0f / output[i][posIdx].w;
		if (normalIdx)
			output[i][normalIdx] *= t;
		if (texCoordIdx)
			output[i][texCoordIdx] *= t;
		output[i][posIdx] *= t;
	}
}

unsigned char *SRDeviceContext::Interpolation(std::vector<glm::vec4> vertex[3], glm::vec3 &bcScreen)
{
	//插值其他信息
	unsigned char *buffer = new unsigned char[100];
	for (int i = 0; i < pPixelShader->inDesc.size(); i++)
	{
		glm::vec4 attribute = Utils::BarycentricLerp(vertex[0][i], vertex[1][i], vertex[2][i], bcScreen); 
		if (pPixelShader->inDesc[i].Name == "SV_TEXCOORD0" || pPixelShader->inDesc[i].Name == "SV_Normal")
		{
			attribute /= attribute.w;
		}
		memcpy(buffer + pPixelShader->inDesc[i].Offset, &attribute, pPixelShader->inDesc[i].Size);
	}
	return buffer;
}

void SRDeviceContext::AlphaBlend(glm::vec4 &color, glm::vec4 dstColor)
{
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

void SRDeviceContext::ParseSrcBlendParam(BLEND blend, glm::vec4 &srcColor, glm::vec4 dstColor)
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
		srcColor = glm::vec4(glm::vec3(srcColor) * srcColor[3] / 255.f, srcColor[3]);
		break;
	case BLEND_INV_SRC_ALPHA:
		srcColor *= (255.f - srcColor[3] / 255.f);
		break;
	case BLEND_DEST_ALPHA:
		srcColor *= dstColor[3] / 255.f;
		break;
	case BLEND_INV_DEST_ALPHA:
		srcColor *= (255.f - dstColor[3] / 255.f);
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

void SRDeviceContext::ParseDstBlendParam(BLEND blend, glm::vec4 srcColor, glm::vec4 &dstColor)
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
	{
		float alpha = (255.f - srcColor[3]) / 255.f;
		dstColor = glm::vec4(glm::vec3(dstColor) * alpha, dstColor[3]);
	}
	break;
	case BLEND_DEST_ALPHA:
		dstColor *= dstColor[3] / 255.f;
		break;
	case BLEND_INV_DEST_ALPHA:
		dstColor *= (255.f - dstColor[3] / 255.f);
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

void SRDeviceContext::BindConstanBuffer()
{
	if (pVertexConstantBuffer)
	{
		pVertexShader->cbuffer = (unsigned char *)realloc(pVertexShader->cbuffer, pVertexConstantBuffer->GetByteWidth());
		memcpy(pVertexShader->cbuffer, pVertexConstantBuffer->GetBuffer(0), pVertexConstantBuffer->GetByteWidth());
	}

	if (pPixelConstantBuffer)
	{
		pPixelShader->cbuffer = (unsigned char *)realloc(pPixelShader->cbuffer, pPixelConstantBuffer->GetByteWidth());
		memcpy(pPixelShader->cbuffer, pPixelConstantBuffer->GetBuffer(0), pPixelConstantBuffer->GetByteWidth());
	}
}

void SRDeviceContext::Resolve()
{
	tbb::parallel_for(0, pViewports->Width, 1, [&](size_t x)
	{
		tbb::parallel_for(0, pViewports->Height, 1, [&](size_t y)
		{
			int idx = x + y * (pViewports->Width - 1);
			glm::vec4 color(0, 0, 0, 0);
			for (int i = 0; i < 4; i++)
			{
				color += msaaBuffer[idx].color[i];
			}
			color /= 4.f;
			if (color == glm::vec4(0, 0, 0, 0)) return;
			pBackBuffer->SetPixel(x, y, color.r, color.g, color.b, color.a);
		});
	});

}

void SRDeviceContext::ClearMSAABuffer()
{
	tbb::parallel_for(0, pViewports->Width * pViewports->Height, 1, [&](size_t idx)
	{
		tbb::parallel_for(0, 4, 1, [&](size_t i)
		{
			msaaBuffer[idx].color[i] = glm::vec4(127, 127, 127, 255);
			msaaBuffer[idx].coverage[i] = 0;
			msaaBuffer[idx].depth[i] = FLT_MAX;
		});
	});
}

MSAAData SRDeviceContext::CoverageCalc(int x, int y, std::vector<glm::vec3> points)
{
	MSAAData data;
	auto IsInsideTriangle = [&](glm::vec2 &pos, int idx)
	{
		glm::vec3 barycentric = Utils::Barycentric(points[0], points[1], points[2], pos);
		if (barycentric[0] < 0 || barycentric[1] < 0 || barycentric[2] < 0)
		{
			data.coverage[idx] = 0;
			data.depth[idx] = 1;
			return;
		}
		data.coverage[idx] = 1;
		data.depth[idx] = Utils::BarycentricLerp(points[0], points[1], points[2], barycentric).z;
		return;
	};
	IsInsideTriangle(glm::vec2(x, y) + glm::vec2(0.25, 0.25), 0);
	IsInsideTriangle(glm::vec2(x, y) + glm::vec2(0.25, 0.75), 1);
	IsInsideTriangle(glm::vec2(x, y) + glm::vec2(0.75, 0.25), 2);
	IsInsideTriangle(glm::vec2(x, y) + glm::vec2(0.75, 0.75), 3);
	return data;
}
