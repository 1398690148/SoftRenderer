#include <algorithm>
#include "SRDeviceContext.h"
#include "tbb/tbb.h"
#include <tbb/tick_count.h>
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
	colorBuffer = (unsigned char *)gFbo;
}

SRDeviceContext::~SRDeviceContext()
{
}

void SRDeviceContext::ClearRenderTargetView(SRRenderTargetView *RenderTargetView, const glm::vec4 &ColorRGBA)
{
	frameTime = 0;
	pBackBuffer->ClearBuffer(ColorRGBA);
}

void SRDeviceContext::ClearDepthStencilView(SRDepthStencilView *DepthStencilView)
{
	DepthStencilView->ClearBuffer(FLT_MAX);
}

void SRDeviceContext::IASetVertexBuffers(SRIBuffer *buf, const unsigned int *pStrides, const unsigned int *pOffsets)
{
	pVertexBuffer = buf;
}

void SRDeviceContext::IASetIndexBuffer(SRIBuffer *buf, unsigned int Offset)
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
	tbb::flow::graph g;
	int count = 0;
	int indiceSize = indices.size();
	tbb::flow::input_node<int> start_node(g, [&](tbb::flow_control &fc) -> int {
		if (count < indiceSize)
		{
			return count++;
		}
		fc.stop();
		return {};
	});
	tbb::flow::function_node<int, std::vector<TriangleData>> vertex_node(g,
		tbb::flow::unlimited, [&](const int i)
	{
		unsigned char *buffer = new unsigned char[160];
		unsigned char *o0 = Vertex(indices[i][0], buffer);
		unsigned char *o1 = Vertex(indices[i][1], buffer);
		unsigned char *o2 = Vertex(indices[i][2], buffer);

		TriangleData vertex(3, VertexData(vertexOutMapTable.size()));
		ParseShaderOutput(o0, vertex[0]);
		ParseShaderOutput(o1, vertex[1]);
		ParseShaderOutput(o2, vertex[2]);

		std::vector<VertexData> clipped_vertices = ClipSutherlandHodgeman(vertex[0], vertex[1], vertex[2], 0.1, 100);
		int len = (int)clipped_vertices.size() - 2;
		std::vector<TriangleData> v;
		for (int i = 0; i < len; i++)
		{
			TriangleData vertex(3);
			vertex[0] = clipped_vertices[0];
			vertex[1] = clipped_vertices[i + 1];
			vertex[2] = clipped_vertices[i + 2];
			PrePerspCorrection(vertex);
			//视口变换
			ViewportTransform(vertex);
			if (shouldCulled(vertex[0][posIdx], vertex[1][posIdx], vertex[2][posIdx])) continue;
			v.push_back(vertex);
		}
		delete o0;
		delete o1;
		delete o2;
		delete buffer;
		return v;
	});
	tbb::flow::function_node<std::vector<TriangleData>> pixel_node(g,
		tbb::flow::unlimited, [&](std::vector<TriangleData> triangleData) {
		for (int i = 0; i < triangleData.size(); i++)
		{
			Triangle(triangleData[i]);
		}
	});
	tbb::flow::make_edge(start_node, vertex_node);
	tbb::flow::make_edge(vertex_node, pixel_node);

	start_node.activate();
	g.wait_for_all();
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

void SRDeviceContext::Triangle(TriangleData vertex)
{
	std::vector<glm::vec3> points = { vertex[0][posIdx], vertex[1][posIdx], vertex[2][posIdx] };

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
	bool alphaFlag = pBlendState->blendDesc.RenderTarget[0].BlendEnable;
	auto edgeFunction = [](const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
	{
		return (c.x - a.x) * (a.y - b.y) - (c.y - a.y) * (a.x - b.x);
	};
	float area = edgeFunction(points[0], points[1], points[2]);

	auto fragment_func = [&](int x, int  y, unsigned char *buffer) -> bool {
		if (x >= pViewports->Width || y >= pViewports->Height) return false;
		MSAAData curFrag;

		auto IsInsideTriangle = [&](glm::vec2 &pos, int idx) -> bool
		{
			float w0 = edgeFunction(points[0], points[1], pos);
			float w1 = edgeFunction(points[1], points[2], pos);
			float w2 = edgeFunction(points[2], points[0], pos);
			if (w0 < 0 || w1 < 0 || w2 < 0)
			{
				curFrag.coverage[idx] = 0;
				curFrag.depth[idx] = FLT_MAX;
				return false;
			}
			curFrag.coverage[idx] = 1;
			curFrag.depth[idx] = Utils::BarycentricLerp(points[0], points[1], points[2], glm::vec3(w1, w2, w0) / area).z;
			return true;
		};

		//0.2
		bool isInside0 = IsInsideTriangle(glm::vec2(x + 0.25, y + 0.25), 0);
		bool isInside1 = IsInsideTriangle(glm::vec2(x + 0.25, y + 0.75), 1);
		bool isInside2 = IsInsideTriangle(glm::vec2(x + 0.75, y + 0.25), 2);
		bool isInside3 = IsInsideTriangle(glm::vec2(x + 0.75, y + 0.75), 3);
		if (!isInside0 && !isInside1 && !isInside2 && !isInside3) return false;

		//0.012
		glm::vec2 P(x + 0.5, y + 0.5);
		float w0 = edgeFunction(points[0], points[1], P);
		float w1 = edgeFunction(points[1], points[2], P);
		float w2 = edgeFunction(points[2], points[0], P);
		glm::vec3 bcScreen = glm::vec3(w1, w2, w0) / area;
		glm::vec4 color(0, 0, 0, 255);

		//0.02 调用函数开销很大
		//Interpolation(buffer, vertex, bcScreen);
		for (int i = 0; i < pPixelShader->inDesc.size(); i++)
		{
			glm::vec4 attribute = Utils::BarycentricLerp(vertex[0][i], vertex[1][i], vertex[2][i], bcScreen);
			attribute /= attribute.w;
			memcpy(buffer + pPixelShader->inDesc[i].Offset, &attribute, pPixelShader->inDesc[i].Size);
		}
		//0.04
		bool discard = pPixelShader->fragment(buffer, color);

		color = glm::vec4(min(255.0f, color[0]), min(255.0f, color[1]), min(255.0f, color[2]), min(255.0f, color[3]));
		//Alpha Test
		if (alphaFlag && discard) return true;

		auto &fragColor = pBackBuffer->GetPixel(x, y);
		auto &fragDepth = pDepthStencilView->GetDepth(x, y);
		for (int i = 0; i < 4; i++)
		{
			if (!curFrag.coverage[i]) continue;
			//Alpha Blend
			if (alphaFlag)
			{
				AlphaBlend(color, fragColor[i]);
				fragColor[i] = color;
			}
			else if (curFrag.depth[i] < fragDepth[i])
			{
				fragColor[i] = color;
				fragDepth[i] = curFrag.depth[i];
			}
		}
		return true;
	};

	auto DDXDDY = [&](int x, int y)
	{
		if (vertexOutMapTable.find("SV_TEXCOORD0") == vertexOutMapTable.end()) return;
		unsigned int texCoordIdx = vertexOutMapTable["SV_TEXCOORD0"];
		QuadFragments quadFragment;
		glm::vec3 texCoord0 = vertex[0][texCoordIdx];
		glm::vec3 texCoord1 = vertex[1][texCoordIdx];
		glm::vec3 texCoord2 = vertex[2][texCoordIdx];
		float w0 = edgeFunction(points[0], points[1], glm::vec2(x + 0.5, y + 0.5));
		float w1 = edgeFunction(points[1], points[2], glm::vec2(x + 0.5, y + 0.5));
		float w2 = edgeFunction(points[2], points[0], glm::vec2(x + 0.5, y + 0.5));
		quadFragment.m_fragments[0] = Utils::BarycentricLerp(texCoord0, texCoord1, texCoord2, glm::vec3(w1 / area, w2 / area, w0 / area));
		quadFragment.m_fragments[0] /= quadFragment.m_fragments[0].z;
		w0 = edgeFunction(points[0], points[1], glm::vec2(x + 1.5, y + 0.5));
		w1 = edgeFunction(points[1], points[2], glm::vec2(x + 1.5, y + 0.5));
		w2 = edgeFunction(points[2], points[0], glm::vec2(x + 1.5, y + 0.5));
		quadFragment.m_fragments[1] = Utils::BarycentricLerp(texCoord0, texCoord1, texCoord2, glm::vec3(w1 / area, w2 / area, w0 / area));
		quadFragment.m_fragments[1] /= quadFragment.m_fragments[1].z;
		w0 = edgeFunction(points[0], points[1], glm::vec2(x + 0.5, y + 1.5));
		w1 = edgeFunction(points[1], points[2], glm::vec2(x + 0.5, y + 1.5));
		w2 = edgeFunction(points[2], points[0], glm::vec2(x + 0.5, y + 1.5));
		quadFragment.m_fragments[2] = Utils::BarycentricLerp(texCoord0, texCoord1, texCoord2, glm::vec3(w1 / area, w2 / area, w0 / area));
		quadFragment.m_fragments[2] /= quadFragment.m_fragments[2].z;
		glm::vec2 ddx(quadFragment.dUdx(), quadFragment.dUdy());
		glm::vec2 ddy(quadFragment.dVdx(), quadFragment.dVdy());
		memcpy(&pPixelShader->dFdx.local(), &ddx, sizeof(glm::vec2));
		memcpy(&pPixelShader->dFdy.local(), &ddy, sizeof(glm::vec2));
	};
	tbb::parallel_for(bboxmin.y, bboxmax.y + 1, 2, [&](size_t y)
	{
		unsigned char *buffer = new unsigned char[100];
		bool isEnter0 = false, isEnd0 = false;
		bool isEnter1 = false, isEnd1 = false;
		for (int x = bboxmin.x; x <= bboxmax.x; x += 2)
		{
			DDXDDY(x, y);
			if (!isEnd0)
			{
				bool flag0 = fragment_func(x, y, buffer);
				fragment_func(x + 1, y, buffer);
				isEnter0 = flag0;
				isEnd0 = isEnter0 && !flag0;
			}
			if (!isEnd1)
			{
				bool flag0 = fragment_func(x, y + 1, buffer);
				fragment_func(x + 1, y + 1, buffer);
				isEnter0 = flag0;
				isEnd0 = isEnter0 && !flag0;
			}
			if (isEnd0 && isEnd1)
			{
				break;
			}
		}
		delete buffer;
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

void SRDeviceContext::ViewportTransform(TriangleData &vertex)
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

void SRDeviceContext::DDXDDY(TriangleData vertex, glm::vec3 &t0, glm::vec3 &t1, glm::vec3 &t2, glm::vec2 &P)
{
	tbb::tick_count time = tbb::tick_count().now();

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
	frameTime += (tbb::tick_count::now() - time).seconds();

}

void SRDeviceContext::PrePerspCorrection(TriangleData &output)
{
	for (int i = 0; i < 3; i++)
	{
		float t = 1.0f / output[i][posIdx].w;
		for (int j = 0; j < output[i].size(); j++)
		{
			output[i][j] *= t;
		}
	}
}

void SRDeviceContext::Interpolation(unsigned char *buffer, TriangleData vertex, glm::vec3 &bcScreen)
{
	//插值其他信息

	for (int i = 0; i < pPixelShader->inDesc.size(); i++)
	{

		glm::vec4 attribute = Utils::BarycentricLerp(vertex[0][i], vertex[1][i], vertex[2][i], bcScreen);
		attribute /= attribute.w;
		memcpy(buffer + pPixelShader->inDesc[i].Offset, &attribute, pPixelShader->inDesc[i].Size);
	}

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
	int width = pViewports->Width;
	int height = pViewports->Height - 1;
	tbb::parallel_for(0, pViewports->Width, 1, [&](size_t x)
	{
		for (int y = 0; y < height; y++)
		{
			int idx = x + (height - y) * width;
			glm::vec4 color(0, 0, 0, 0);
			auto &fragCache = pBackBuffer->GetPixel(x, y);
			for (int i = 0; i < 4; i++)
			{
				color += fragCache[i];
			}
			color /= 4.f;
			colorBuffer[idx * 4] = color.b;
			colorBuffer[idx * 4 + 1] = color.g;
			colorBuffer[idx * 4 + 2] = color.r;
		}
	});

	std::cout << frameTime << std::endl;
}