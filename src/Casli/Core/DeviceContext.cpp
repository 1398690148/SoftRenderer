#include <algorithm>
#include "DeviceContext.h"
#include "IVertexShader.h"

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

void DeviceContext::IASetVertexBuffers(IBuffer * buf, const unsigned int *pStrides, const unsigned int *pOffsets)
{
	pVertexBuffer = buf;
}

void DeviceContext::IASetIndexBuffer(IBuffer * buf, unsigned int Offset)
{
	pIndexBuffer = buf;
}

void DeviceContext::RSSetViewports(unsigned int NumViewports, const VIEWPORT * Viewports)
{
	pViewports = Viewports;
}

void DeviceContext::VSSetShader(IVertexShader * shader)
{
	pVertexShader = shader;
}

void DeviceContext::PSSetShader(IPixelShader * shader)
{
	pPixelShader = shader;
}

void DeviceContext::IASetInputLayout(InputLayout *InputLayout)
{
	pInputLayout = InputLayout;
}

void DeviceContext::IASetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology)
{
	pTopology = topology;
}

void DeviceContext::OMSetRenderTargets(RenderTargetView **RenderTargetView)
{
	pRenderTargetView = *RenderTargetView;
}

void DeviceContext::DrawIndex()
{
	//if (!pVertexBuffer || !pIndexBuffer || !pInputLayout || !pPixelShader || !pPixelShader || !pRenderTargetView) return;
	int positionOffset = 0, positionNum = 0, positionSize = 0;
	if (!ParseVertexBuffer("Position", positionOffset, positionNum, positionSize))
	{
		return;
	}

	int colorOffset = 0, colorNum = 0, colorSize = 0;
	bool hasColor = ParseVertexBuffer("Color", colorOffset, colorNum, colorSize);
	
	//获取一个片元的索引
	for (unsigned int offset = 0; offset < pIndexBuffer->GetByteWidth();)
	{
		unsigned int *indexBuffer = (unsigned int *)pIndexBuffer->GetBuffer(offset);
		offset += pIndexBuffer->GetStructureByteStride() * 3;

		switch (pInputLayout->getData(positionOffset)->Size / 4)
		{
		case 2:
		{
			float p0x = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[0] + positionOffset)) * pViewports->Width;
			float p0y = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[0] + positionOffset + 4)) * pViewports->Height;
			float p1x = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[1] + positionOffset)) * pViewports->Width;
			float p1y = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[1] + positionOffset + 4)) * pViewports->Height;
			float p2x = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[2] + positionOffset)) * pViewports->Width;
			float p2y = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[2] + positionOffset + 4)) * pViewports->Height;

			Vec4f *color = new Vec4f[4];
			if (hasColor)
			{
				for (int i = 0; i < 3; i++)
				{
					color[i][3] = 255;
					{
						if (colorSize == 1)
						{
							if (colorNum == 3)
							{
								color[i][0] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset));
								color[i][1] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + colorSize));
								color[i][2] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + 2 * colorSize));
							}
							else if (colorNum == 4)
							{
								color[i][0] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset));
								color[i][1] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + colorSize));
								color[i][2] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + 2 * colorSize));
								color[i][3] = *((unsigned char *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + 3 * colorSize));
							}
						}
						else if (colorSize == 4)
						{
							if (colorNum == 3)
							{
								color[i][0] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset));
								color[i][1] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + colorSize));
								color[i][2] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + 2 * colorSize));
							}
							else if (colorNum == 4)
							{
								color[i][0] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset));
								color[i][1] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + colorSize));
								color[i][2] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + 2 * colorSize));
								color[i][3] = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[i] + colorOffset + 3 * colorSize));
							}
						}
					}
				}
			}
			if (pTopology == PRIMITIVE_TOPOLOGY::PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			{
				triangle(Vec2i(p0x, p0y), Vec2i(p1x, p1y), Vec2i(p2x, p2y), color);
			}
		}
		break;
		case 3:
		{
			float p0x = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[0] + positionOffset));
			float p0y = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[0] + positionOffset + 4));
			float p0z = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[0] + positionOffset + 8));
			float p1x = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[1] + positionOffset));
			float p1y = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[1] + positionOffset + 4));
			float p1z = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[1] + positionOffset + 8));
			float p2x = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[2] + positionOffset));
			float p2y = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[2] + positionOffset + 4));
			float p2z = *((float *)pVertexBuffer->GetBuffer(pVertexBuffer->GetStructureByteStride() * indexBuffer[2] + positionOffset + 8));
			Vec4f color;
			color[3] = 1.0f;
			if (pTopology == PRIMITIVE_TOPOLOGY::PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			{
				triangle(Vec3i(p0x, p0y, p0z), Vec3i(p1x, p1y, p1z), Vec3i(p2x, p2y, p2z), color);
			}
		}
		break;
		}
	}
}

Vec3f barycentric(Vec2i pts0, Vec2i pts1, Vec2i pts2, Vec2i P) {
	Vec3f u = cross(Vec3f(pts2[0] - pts0[0], pts1[0] - pts0[0], pts0[0] - P[0]), Vec3f(pts2[1] - pts0[1], pts1[1] - pts0[1], pts0[1] - P[1]));
	/* `pts` and `P` has integer value as coordinates
	   so `abs(u[2])` < 1 means `u[2]` is 0, that means
	   triangle is degenerate, in this case return something with negative coordinates */
	if (std::abs(u[2]) < 1) return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void DeviceContext::triangle(Vec2i t0, Vec2i t1, Vec2i t2, Vec4f color[3])
{
	if (t0.y == t1.y && t0.y == t2.y) return; // I dont care about degenerate triangles 
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	//排序顶点时不要忘记排序颜色，否则顶点颜色对应不上
	if (t0.y > t1.y)
	{
		std::swap(t0, t1);
		std::swap(color[0], color[1]);
	}
	if (t0.y > t2.y)
	{
		std::swap(t0, t2);
		std::swap(color[0], color[2]);
	}
	if (t1.y > t2.y)
	{
		std::swap(t1, t2);
		std::swap(color[1], color[2]);
	}
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
		Vec2i A = t0 + (t2 - t0)*alpha;
		Vec2i B = second_half ? t1 + (t2 - t1)*beta : t0 + (t1 - t0)*beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			Vec2i P(j, t0.y + i);
			Vec3f bcScreen = barycentric(t0, t1, t2, P);
			Vec4f colors =  color[0] * bcScreen[0] + color[1] * bcScreen[1] + color[2] * bcScreen[2];
			pRenderTargetView->SetPixel(j, t0.y + i, colors[0], colors[1], colors[2], colors[3]);
		}
	}
}

//Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2i P)
//{
//	Vec3f s[2];
//	for (int i = 1; i >= 0; i--)
//	{
//		s[i][0] = C[i] - A[i];
//		s[i][1] = B[i] - A[i];
//		s[i][2] = A[i] - P[i];
//	}
//	Vec3f u = cross(s[0], s[1]);
//	if (std::abs(u[2]) > 1e-2)
//		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
//	return Vec3f(-1, 1, 1);
//}
//
//void DeviceContext::triangle(Vec3f *pts, Vec4f *color)
//{
//	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
//	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			bboxmin[j] = std::min(bboxmin[j], pts[i][j]);
//			bboxmax[j] = std::max(bboxmax[j], pts[i][j]);
//		}
//	}
//	Vec2i P;
//	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
//	{
//		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
//		{
//			Vec3f bcScreen = barycentric(proj<2>(pts[0]), proj<2>(pts[1]), proj<2>(pts[2]), proj<2>(P));
//			//float z = pts[0][2] * bcScreen.x + pts[1][2] * bcScreen.y + pts[2][2] * bcScreen.z;
//			//float w = pts[0][3] * bcScreen.x + pts[1][3] * bcScreen.y + pts[2][3] * bcScreen.z;
//			//float fragDepth = std::max(0, std::min(255, int(z / w + 0.5)));
//			if (bcScreen.x < 0 || bcScreen.y < 0 || bcScreen.z < 0) continue;
//			Vec4f colors = color[0] * bcScreen[0] + color[1] * bcScreen[1] + color[2] * bcScreen[2];
//			pRenderTargetView->SetPixel(P.x, P.y, colors[0], colors[1], colors[2], colors[3]);
//		}
//	}
//}

void DeviceContext::triangle(Vec3i t0, Vec3i t1, Vec3i t2, Vec4f color)
{
	
}

bool DeviceContext::ParseVertexBuffer(std::string semanticName, int &offset, int &num, int &size)
{
	unsigned int inputSize = sizeof(INPUT_ELEMENT_DESC);
	//Color在VertexBuffer中的偏移
	bool hasColor = true;
	int idx = 0;
	for (int i = 0; i < pInputLayout->getSize(); i++)
	{
		std::string str = pInputLayout->getData(idx)->SemanticName;
		if (pInputLayout->getData(idx)->SemanticName == semanticName)
		{
			num = pInputLayout->getData(idx)->Size / pInputLayout->getData(idx)->typeSize;
			break;
		}
		idx++;
	}
	if (idx >= pInputLayout->getSize())
	{
		hasColor = false;
	}
	else
	{
		offset = pInputLayout->getData(idx)->Offset;
		num = pInputLayout->getData(idx)->Size / pInputLayout->getData(idx)->typeSize;
		size = pInputLayout->getData(idx)->typeSize;
	}
	return hasColor;
}
