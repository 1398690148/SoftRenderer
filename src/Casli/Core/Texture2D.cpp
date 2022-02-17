#include "Texture2D.h"

Texture2D::Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd)
	: IBuffer(sizeof(float) * desc->Width * desc->Height, sizeof(float)), width(desc->Width), height(desc->Height),
	mipLevels(desc->MipLevels), format(desc->Format)
{
	if (sd)
	{
		m_Buffer = (unsigned char *)sd->pSysMem;
		SysMemPitch = sd->SysMemPitch;
	}
	else
	{
		m_Buffer = new unsigned char[width * height * 4];
		memset(m_Buffer, 0, width * height * 4);
	}
}

Texture2D::Texture2D(TEXTURE2D_DESC * desc, SUBRESOURCE_DATA * sd, IBuffer *pDepthBuffer)
	: IBuffer(sizeof(float), desc->Width * desc->Height), width(desc->Width), height(desc->Height),
	mipLevels(desc->MipLevels), format(desc->Format)
{
	if (pDepthBuffer)
	{
		m_Buffer = (unsigned char *)(pDepthBuffer->GetBuffer(0));
	}
	else
	{
		m_Buffer = new unsigned char[ByteWidth];
		memset(m_Buffer, -1, ByteWidth);
	}
}

Texture2D::~Texture2D()
{
}

void Texture2D::GenerateMips()
{
}

Vec4f Texture2D::Sampler(Vec2f uv, SamplerState *sampler)
{
	int x = uv.x * (width - 1);
	int y = uv.y * (height - 1);
	int index = (x + y * width) * 3;
	return Vec4f(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
}
