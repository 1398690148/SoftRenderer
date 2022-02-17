#include "DepthStencilView.h"
#include <memory>

DepthStencilView::DepthStencilView(Texture2D *buffer)
	: IBuffer(buffer->GetWidth() * buffer->GetHeight(), buffer->GetStructureByteStride()), width(buffer->GetWidth()), height(buffer->GetHeight())
{
	if (buffer)
	{
		m_Buffer = (unsigned char *)(buffer->GetBuffer(0));
	}
	else
	{
		m_Buffer = new unsigned char[ByteWidth];
	}
	memset(m_Buffer, 0, ByteWidth);
	//for (int i = 0; i < ByteWidth; i++)
	//{
	//	m_Buffer[i] = 255;
	//}
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

unsigned char DepthStencilView::GetDepth(int i, int j)
{
	return *(m_Buffer + i + (height - 1 - j) * width);
}

void DepthStencilView::SetDepth(int i, int j, unsigned char depth)
{
	m_Buffer[i + (height - 1 - j) * width] = depth;
}
