#include "DepthStencilView.h"
#include <memory>

DepthStencilView::DepthStencilView(Texture2D *buffer)
	: IBuffer(buffer->GetWidth() * buffer->GetHeight() * 4, buffer->GetStructureByteStride()), 
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
	for (int i = buffer->GetWidth() * buffer->GetHeight() - 1; i >= 0; i--)
	{
		((float *)m_Buffer)[i] = FLT_MAX;
	}
}

void DepthStencilView::ClearBuffer(const float depth)
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

float DepthStencilView::GetDepth(int i, int j)
{
	return ((float *)m_Buffer)[i + (maxHeight - j) * width];
}

void DepthStencilView::SetDepth(int i, int j, float depth)
{
	((float *)m_Buffer)[i + (maxHeight - j) * width] = depth;
}
