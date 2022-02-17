#include "Texture2D.h"
#include "SamplerState.h"
#include <algorithm>

Texture2D::Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd)
	: IBuffer(desc->Width * desc->Height, sizeof(unsigned char)), width(desc->Width), height(desc->Height),
	mipLevels(desc->MipLevels), format(desc->Format)
{
	if (sd)
	{
		m_Buffer = (unsigned char *)sd->pSysMem;
		SysMemPitch = sd->SysMemPitch;
		channels = SysMemPitch / width;
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

Vec4f Texture2D::Bilinear(const float &tx, const float  &ty,
	const Vec4f &c00, const Vec4f &c10, const Vec4f &c01, const Vec4f &c11)
{
#if 1 
	Vec4f  a = c00 * (1 - tx) + c10 * tx;
	Vec4f  b = c01 * (1 - tx) + c11 * tx;
	return a * (1 - ty) + b * ty;
#else 
	return c00 * (1 - tx) * (1 - ty) +
		c10 * tx * (1 - ty) +
		c01 * (1 - tx) * ty +
		c11 * tx * ty;
#endif
}

Vec4f Texture2D::Sampler(Vec2f uv, SamplerState *sampler)
{
	int x = uv.x * (width - 1);
	int y = uv.y * (height - 1);

	switch (channels)
	{
	case 1:
	{
		if (sampler->pSamplerState.Filter == FILTER_POINT)
		{
			int index = (x + y * width);
			return Vec4f(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
		}
		else if (sampler->pSamplerState.Filter == FILTER_LINEAR)
		{
			int index = (x + y * width);
			unsigned int maxIndex = 4 * (width - 1) * (height - 1) - 4;
			const Vec4f c00 = Vec4f(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
			index = std::min(maxIndex, (x + y * width + 1));
			const Vec4f c10 = Vec4f(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
			index = std::min(maxIndex, (x + (y + 1) * width));
			const Vec4f c01 = Vec4f(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
			index = std::min(maxIndex, (x + 1 + (y + 1) * width));
			const Vec4f c11 = Vec4f(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
			return Bilinear(uv.x, uv.y, c00, c10, c01, c11);
		}
	}
	break;
	case 4:
	{
		if (sampler->pSamplerState.Filter == FILTER_POINT)
		{
			int index = (x + y * width) * 4;
			return Vec4f(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
		}
		else if (sampler->pSamplerState.Filter == FILTER_LINEAR)
		{
			int index = (x + y * width) * 4;
			unsigned int maxIndex = 4 * (width - 1) * (height - 1) - 4;
			const Vec4f c00 = Vec4f(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
			index = std::min(maxIndex, (x + y * width + 1) * 4);
			const Vec4f c10 = Vec4f(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
			index = std::min(maxIndex, (x + (y + 1) * width) * 4);
			const Vec4f c01 = Vec4f(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
			index = std::min(maxIndex, (x + 1 + (y + 1) * width) * 4);
			const Vec4f c11 = Vec4f(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
			return Bilinear(uv.x, uv.y, c00, c10, c01, c11);
		}
	}
		break;
	}

	
	return Vec4f(0, 0, 0, 0);
}
