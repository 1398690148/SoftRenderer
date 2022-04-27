#include "SRRenderTargetView.h"
#include <tbb/parallel_for.h>
#include <execution>
#include <tbb/tick_count.h>
#include <iostream>

SRRenderTargetView::SRRenderTargetView(unsigned int w, unsigned int h)
	: width(w), height(h), length(w * h)
{
	m_Buffer.resize(length);
}

void SRRenderTargetView::ClearBuffer(glm::u8vec4 ColorRGBA)
{
	std::array<glm::u8vec4, 4> clearColor;
	clearColor.fill(ColorRGBA);
	tbb::parallel_for(0, length, 1, [&](size_t i) { m_Buffer[i] = clearColor; });
}

void SRRenderTargetView::SetPixel(int i, int j, int k, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	int index = i + j * width;
	(m_Buffer[index].begin())[k][0] = r;
	(m_Buffer[index].begin())[k][1] = g;
	(m_Buffer[index].begin())[k][2] = b;
	(m_Buffer[index].begin())[k][3] = a;
}

std::array<glm::u8vec4, 4> &SRRenderTargetView::GetPixel(int i, int j)
{
	return m_Buffer[i + j * width];
}

std::array<glm::u8vec4, 4>& SRRenderTargetView::GetPixel(int index)
{
	return m_Buffer[index];
}