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
		//for (auto &iter : m_Buffer[i])
		//{
			//iter = depth;
		//}
		m_Buffer[i] = clearDepth;
	});
	//unsigned int stride = sizeof(float);
	//float *buffer = (float *)m_Buffer;
	//int start = 0;
	//while (start < ByteWidth)
	//{
	//	*buffer++ = depth;
	//	start += stride;
	//}
}

std::array<float, 4> &SRDepthStencilView::GetDepth(int i, int j)
{
	return m_Buffer[i + j * width];
}

void SRDepthStencilView::SetDepth(int i, int j, int k, float depth)
{
	*(m_Buffer[i + j * width].begin() + k) = depth;
}
