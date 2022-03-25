#include "SRRenderTargetView.h"
#include <tbb/parallel_for.h>

SRRenderTargetView::SRRenderTargetView(unsigned int w, unsigned int h)
	: width(w), height(h)
{
	m_Buffer.resize(w * h);
}

void SRRenderTargetView::ClearBuffer(const glm::vec4 &ColorRGBA)
{
	std::array<glm::ivec4, 4> clearColor;
	clearColor.fill(ColorRGBA);
	tbb::parallel_for(0, (int)(width * height), [&](size_t i) {
		m_Buffer[i] = clearColor;
	});
}

void SRRenderTargetView::SetPixel(int i, int j, int k, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	r = r > 255 ? 255 : r;
	g = g > 255 ? 255 : g;
	b = b > 255 ? 255 : b;
	a = a > 255 ? 255 : a;
	int index = i + j * width;
	*(m_Buffer[index].begin() + k) = glm::ivec4(r, g, b, a);
}

std::array<glm::ivec4, 4> &SRRenderTargetView::GetPixel(int i, int j)
{
	return m_Buffer[i + j * width];
}

std::array<glm::ivec4, 4>& SRRenderTargetView::GetPixel(int index)
{
	return m_Buffer[index];
}