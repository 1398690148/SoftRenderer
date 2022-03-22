#include "SRDepthStencilView.h"
#include <memory>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

SRDepthStencilView::SRDepthStencilView(SRTexture2D *buffer)
	: SRIBuffer(buffer->GetWidth() * buffer->GetHeight() * 4, buffer->GetStructureByteStride()), 
	width(buffer->GetWidth()), height(buffer->GetHeight())
{
	if (buffer)
	{
		buffer->GetUniqueBuffer(&m_Buffer);
	}
	//else
	//{
	//	m_Buffer = new unsigned char[ByteWidth];
	//}
	int size = width * height;
	tbb::parallel_for(0, size, [&](size_t r)
	{
		((float *)m_Buffer)[r] = FLT_MAX;
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
	return  ((float *)m_Buffer)[i + (height - 1 - j) * width];
}

void SRDepthStencilView::SetDepth(int i, int j, float depth)
{
	((float *)m_Buffer)[i + (height - 1 - j) * width] = depth;
}
