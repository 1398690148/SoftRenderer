#include "DepthStencilView.h"
#include <memory>

DepthStencilView::DepthStencilView(Texture2D *buffer)
	: IBuffer(buffer->GetWidth() * buffer->GetHeight() * 4, buffer->GetStructureByteStride()), width(buffer->GetWidth()), height(buffer->GetHeight())
{
	if (buffer)
	{
		m_Buffer = (unsigned char *)(buffer->GetBuffer(0));
	}
	else
	{
		m_Buffer = new unsigned char[ByteWidth];
		memset(m_Buffer, -1, ByteWidth);
	}
	memset(m_Buffer, 0, width * height * 4);
}

void DepthStencilView::ClearBuffer(const unsigned char depth)
{
	unsigned int stride = sizeof(unsigned char) * 4;
	unsigned char *buffer = m_Buffer;
	int start = 0;
	while (start < ByteWidth)
	{
		memcpy(buffer, &depth, stride);
		start += stride;
		buffer += stride;
	}
}

float DepthStencilView::GetDepth(int i, int j)
{
	return *(float *)(m_Buffer + i + (height - j) * width);
}

void DepthStencilView::SetDepth(int i, int j, float depth)
{
	*(float *)(m_Buffer + i + (height - j) * width) = depth;
}

void DepthStencilView::SetPixel(int i, int j, unsigned char r)
{
	unsigned int stride = sizeof(unsigned char) * 4;
	unsigned char *buffer = m_Buffer + i * StructureByteStride + (height - j) * width * StructureByteStride;
	//memcpy(buffer, color, stride);
}
