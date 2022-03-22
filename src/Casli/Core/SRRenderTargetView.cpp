#include "SRRenderTargetView.h"
#include <memory>

SRRenderTargetView::SRRenderTargetView(unsigned int w, unsigned int h, unsigned int structureByteStride)
	: SRIBuffer(w * h * structureByteStride, structureByteStride), width(w), height(h)
{
	m_Buffer = new unsigned char[ByteWidth];
}

SRRenderTargetView::SRRenderTargetView(unsigned int w, unsigned int h, unsigned int structureByteStride, void * gFbo)
	: SRIBuffer(w * h * structureByteStride, structureByteStride), width(w), height(h)
{
	m_Buffer = (unsigned char *)gFbo;
}

void SRRenderTargetView::ClearBuffer(const float ColorRGBA[4])
{
	unsigned int stride = sizeof(unsigned char) * StructureByteStride;
	unsigned char *buffer = m_Buffer;
	unsigned char color[4];
	color[0] = (unsigned char)(ColorRGBA[2] * 255);
	color[1] = (unsigned char)(ColorRGBA[1] * 255);
	color[2] = (unsigned char)(ColorRGBA[0] * 255);
	color[3] = (unsigned char)(ColorRGBA[3] * 255);
	int start = 0;
	while (start < ByteWidth)
	{
		memcpy(buffer, color, stride);
		start += stride;
		buffer += stride;
	}
}

void SRRenderTargetView::SetPixel(int i, int j, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;
	a = a > 255 ? 255 : a;
	unsigned char *buffer = m_Buffer + (i + (height - 1 - j) * width) * StructureByteStride;
	unsigned char color[4] = {b, g, r, a};
	memcpy(buffer, color, StructureByteStride);
}

glm::vec4 SRRenderTargetView::GetPixel(int i, int j)
{
	unsigned int stride = sizeof(unsigned char) * StructureByteStride;
	unsigned char *buffer = m_Buffer + i * StructureByteStride + (height - 1 - j) * width * StructureByteStride;
	return glm::vec4(buffer[0], buffer[1], buffer[2], buffer[3]);
}
