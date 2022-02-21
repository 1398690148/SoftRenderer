#include "Texture2D.h"
#include "SamplerState.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <cmath>
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
	: IBuffer(desc->Width * desc->Height, sizeof(float)), width(desc->Width), height(desc->Height),
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

glm::vec4 Texture2D::Bilinear(const float &tx, const float  &ty,
	const glm::vec4 &c00, const glm::vec4 &c10, const glm::vec4 &c01, const glm::vec4 &c11)
{
#if 1 
	glm::vec4  a = c00 * (1 - tx) + c10 * tx;
	glm::vec4  b = c01 * (1 - tx) + c11 * tx;
	return a * (1 - ty) + b * ty;
#else 
	return c00 * (1 - tx) * (1 - ty) +
		c10 * tx * (1 - ty) +
		c01 * (1 - tx) * ty +
		c11 * tx * ty;
#endif
}

void Texture2D::RemapUV(float &texCoord, TEXTURE_ADDRESS_MODE address)
{
	switch (address)
	{
	case TEXTURE_ADDRESS_WRAP:
		texCoord = texCoord - floor(texCoord);
		break;
	case TEXTURE_ADDRESS_CLAMP:
		texCoord = std::clamp(texCoord, 0.f, 1.f);
		break;
	case TEXTURE_ADDRESS_BORDER:
		texCoord = (texCoord < 0 || texCoord > 1) ? -1 : texCoord;
		break;
	case TEXTURE_ADDRESS_MIRROR:
		if (texCoord > 1)
		{
			texCoord = ((int)floor(texCoord) & 1) ? 1 + floor(texCoord) - texCoord : texCoord - floor(texCoord);
		}
		else if (texCoord < 0)
		{
			texCoord = abs(texCoord);
			texCoord = ((int)floor(texCoord) & 1) ? texCoord - floor(texCoord) : 1 + floor(texCoord) - texCoord;
		}
		break;
	}
}

glm::vec4 Texture2D::Sampler(glm::vec2 uv, SamplerState *sampler)
{
	//RemapUV(uv.r, sampler->pSamplerState.AddressU);
	//RemapUV(uv.t, sampler->pSamplerState.AddressV);


	//if (uv.r < 0 || uv.t < 0)
	//{
	//	unsigned char *color = sampler->pSamplerState.borderColor;
	//	return glm::vec4(color[0], color[1], color[2], color[3]);
	//}

	float gx = uv.x * (width - 1);
	float gy = uv.y * (height - 1);

	int x = uv.x * (width - 1);
	int y = uv.y * (height - 1);

	switch (channels)
	{
	case 1:
	{
		if (sampler->pSamplerState.Filter == FILTER_POINT)
		{
			int index = (x + y * width);
			return glm::vec4(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
		}
		else if (sampler->pSamplerState.Filter == FILTER_LINEAR)
		{
			int index = (x + y * width);
			unsigned int maxIndex = 4 * (width - 1) * (height - 1) - 4;
			const glm::vec4 c00 = glm::vec4(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
			index = std::min(maxIndex, (x + y * width + 1));
			const glm::vec4 c10 = glm::vec4(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
			index = std::min(maxIndex, (x + (y + 1) * width));
			const glm::vec4 c01 = glm::vec4(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
			index = std::min(maxIndex, (x + 1 + (y + 1) * width));
			const glm::vec4 c11 = glm::vec4(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
			return Bilinear(gx - x, gy - y, c00, c10, c01, c11);
		}
	}
	break;
	case 3:
	{
		if (sampler->pSamplerState.Filter == FILTER_POINT)
		{
			int index = (x + y * width) * 3;
			return glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
		}
		else if (sampler->pSamplerState.Filter == FILTER_LINEAR)
		{
			int index = (x + y * width);
			unsigned int maxIndex = 3 * (width - 1) * (height - 1) - 3;
			const glm::vec4 c00 = glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
			index = std::min(maxIndex, (x + y * width + 1));
			const glm::vec4 c10 = glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
			index = std::min(maxIndex, (x + (y + 1) * width));
			const glm::vec4 c01 = glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
			index = std::min(maxIndex, (x + 1 + (y + 1) * width));
			const glm::vec4 c11 = glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
			return Bilinear(gx - x, gy - y, c00, c10, c01, c11);
		}
	}
	break;
	case 4:
	{
		int index = (x + y * width) * 4;
		if (sampler->pSamplerState.Filter == FILTER_POINT)
		{
			return glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], m_Buffer[index + 3]);
		}
		else if (sampler->pSamplerState.Filter == FILTER_LINEAR)
		{
			unsigned int maxIndex = 4 * (width - 1) * (height - 1) - 4;
			const glm::vec4 c00 = glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], m_Buffer[index + 3]);
			index = std::min(maxIndex, (x + y * width + 1) * 4);
			const glm::vec4 c10 = glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], m_Buffer[index + 3]);
			index = std::min(maxIndex, (x + (y + 1) * width) * 4);
			const glm::vec4 c01 = glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], m_Buffer[index + 3]);
			index = std::min(maxIndex, (x + 1 + (y + 1) * width) * 4);
			const glm::vec4 c11 = glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], m_Buffer[index + 3]);
			return Bilinear(uv.x, uv.y, c00, c10, c01, c11);
		}
	}
	break;
	}
	return glm::vec4(255, 255, 255, 255);
}
