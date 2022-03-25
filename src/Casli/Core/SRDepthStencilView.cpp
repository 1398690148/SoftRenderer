#include "SRDepthStencilView.h"
#include <tbb/parallel_for.h>

SRDepthStencilView::SRDepthStencilView(unsigned int w, unsigned int h)
	: width(w), height(h)
{
	m_Buffer.resize(w * h);
}

void SRDepthStencilView::ClearBuffer(const float depth)
{
	std::array<float, 4> clearDepth;
	clearDepth.fill(depth);
	tbb::parallel_for(0, (int)(width * height), [&](size_t i) {
		m_Buffer[i] = clearDepth;
	});

}

std::array<float, 4> &SRDepthStencilView::GetDepth(int i, int j)
{
	return m_Buffer[i + j * width];
}

void SRDepthStencilView::SetDepth(int i, int j, int k, float depth)
{
	*(m_Buffer[i + j * width].begin() + k) = depth;
}
