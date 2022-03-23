#include "SRRenderTargetView.h"
#include <memory>
#include <tbb/parallel_for.h>

SRRenderTargetView::SRRenderTargetView(unsigned int w, unsigned int h)
	: width(w), height(h)
{
	m_Buffer.resize(w * h);
}

//SRRenderTargetView::SRRenderTargetView(unsigned int w, unsigned int h, unsigned int structureByteStride, void *gFbo)
//	: SRIBuffer(w * h * structureByteStride, structureByteStride), width(w), height(h)
//{
//	m_Buffer.data() = gFbo;
//}

void SRRenderTargetView::ClearBuffer(const glm::vec4 &ColorRGBA)
{
	glm::i8vec4 clearColor = ColorRGBA;
	tbb::parallel_for(0, (int)(width * height), [&](size_t i) {
		for (auto &iter : m_Buffer[i])
		{
			iter = clearColor;
		}
	});
	/*unsigned int stride = sizeof(unsigned char) * StructureByteStride;
	unsigned char *buffer = m_Buffer;
	unsigned char color[4];
	color[0] = ColorRGBA[2];
	color[1] = ColorRGBA[1];
	color[2] = ColorRGBA[0];
	color[3] = ColorRGBA[3];
	int start = 0;
	while (start < ByteWidth)
	{
		memcpy(buffer, color, stride);
		start += stride;
		buffer += stride;
	}*/
}

void SRRenderTargetView::SetPixel(int i, int j, int k, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;
	a = a > 255 ? 255 : a;
	int index = i + j * width;
	*(m_Buffer[index].begin() + k) = glm::i8vec4(r, g, b, a);
	/*unsigned char *buffer = m_Buffer + () * StructureByteStride;
	unsigned char color[4] = {b, g, r, a};
	memcpy(buffer, color, StructureByteStride);*/
}

std::array<glm::i8vec4, 4> &SRRenderTargetView::GetPixel(int i, int j)
{
	int index = i + j * width;
	return m_Buffer[index];
	//unsigned int stride = sizeof(unsigned char) * StructureByteStride;
	//unsigned char *buffer = m_Buffer + i * StructureByteStride + (height - 1 - j) * width * StructureByteStride;
	//return glm::vec4(buffer[0], buffer[1], buffer[2], buffer[3]);
}
