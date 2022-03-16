#include "SRDepthStencilView.h"
#include <memory>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

SRDepthStencilView::SRDepthStencilView(SRTexture2D *buffer)
	: SRIBuffer(buffer->GetWidth() * buffer->GetHeight() * 4, buffer->GetStructureByteStride()), 
	width(buffer->GetWidth()), height(buffer->GetHeight()), maxHeight(buffer->GetHeight() - 1)
{
	if (buffer)
	{
		buffer->GetUniqueBuffer(&m_Buffer);
	}
	else
	{
		m_Buffer = new unsigned char[ByteWidth];
	}
	tbb::parallel_for(tbb::blocked_range<size_t>(buffer->GetWidth() * buffer->GetHeight() - 1, 0), [&](const tbb::blocked_range<size_t> &r)
	{
		for (int i = r.begin(); i >= r.end(); i--)
		{
			((float *)m_Buffer)[i] = FLT_MAX;
		}
	});
}

void SRDepthStencilView::ClearBuffer(const float depth)
{
	unsigned int stride = sizeof(float);
	float *buffer = (float *)m_Buffer;
	int start = 0;
	while (start < ByteWidth)
	{
		*buffer++ = depth;
		start += stride;
	}
}

float SRDepthStencilView::GetDepth(int i, int j)
{
	return ((float *)m_Buffer)[i + (maxHeight - j) * width];
}

void SRDepthStencilView::SetDepth(int i, int j, float depth)
{
	((float *)m_Buffer)[i + (maxHeight - j) * width] = depth;
}