#include "SRTexture2D.h"
#include "SRSamplerState.h"
#include <algorithm>
#include <glm/glm.hpp>

SRTexture2D::SRTexture2D(TEXTURE2D_DESC *desc)
	: SRIBuffer(desc->Width * desc->Height * 4, sizeof(unsigned char)),
	width(desc->Width), height(desc->Height), mipLevels(desc->MipLevels)
{
	int size = ByteWidth;
	if (mipLevels > 1)
	{
		size = size * 3 / 2;
	}
	m_Buffer = new unsigned char[size];
	memset(m_Buffer, 0, size);
}

SRTexture2D::SRTexture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd)
	: SRIBuffer(sd->SysMemPitch * desc->Height, sizeof(unsigned char)), 
	width(desc->Width), height(desc->Height), mipLevels(desc->MipLevels)
{
	m_Buffer = (unsigned char *)sd->pSysMem;
	if (mipLevels > 1)
	{
		m_Buffer = (unsigned char *)realloc(m_Buffer, ByteWidth * 3 / 2);
	}
	channels = sd->SysMemPitch / width;
}

SRTexture2D::~SRTexture2D()
{
}

void SRTexture2D::GenerateMips()
{
	int currentPos = ByteWidth;
	int startPos = 0;
	int w = width;
	int h = height;
	for (int k = 1, coe = 1; k <= mipLevels; k++, coe *= 2)
	{
		for (int j = 0; j < h; j += 2)
		{
			for (int i = 0; i < w; i += 2)
			{
				int index0 = startPos + (i + j * w) * channels;
				int index1 = startPos + ((i + 1) + j * w) * channels;
				int index2 = startPos + (i + (j + 1) * w) * channels;
				int index3 = startPos + ((i + 1) + (j + 1) * w) * channels;
				glm::vec4 c00 = glm::vec4(m_Buffer[index0], m_Buffer[index0 + 1], m_Buffer[index0 + 2], channels > 3 ? m_Buffer[index0 + 3] : 255);
				glm::vec4 c01 = glm::vec4(m_Buffer[index1], m_Buffer[index1 + 1], m_Buffer[index1 + 2], channels > 3 ? m_Buffer[index1 + 3] : 255);
				glm::vec4 c10 = glm::vec4(m_Buffer[index2], m_Buffer[index2 + 1], m_Buffer[index2 + 2], channels > 3 ? m_Buffer[index2 + 3] : 255);
				glm::vec4 c11 = glm::vec4(m_Buffer[index3], m_Buffer[index3 + 1], m_Buffer[index3 + 2], channels > 3 ? m_Buffer[index3 + 3] : 255);
				m_Buffer[currentPos++] = (c00 + c01 + c10 + c11).r / 4;
				if (channels > 1)
				{
					m_Buffer[currentPos++] = (c00 + c01 + c10 + c11).g / 4;
					m_Buffer[currentPos++] = (c00 + c01 + c10 + c11).b / 4;
				}
				if (channels > 3)
				{
					m_Buffer[currentPos++] = (c00 + c01 + c10 + c11).a / 4;
				}
			}
		}
		startPos += (ByteWidth / (coe * coe));
		w = std::max(1, w / 2);
		h = std::max(1, h / 2);
		if (w == 1 && h == 1)
		{
			break;
		}
	}
}

glm::vec4 SRTexture2D::Bilinear(const float &tx, const float  &ty,
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

void SRTexture2D::RemapUV(float &texCoord, TEXTURE_ADDRESS_MODE address)
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

glm::vec4 SRTexture2D::Sampler(glm::vec2 uv, SRSamplerState *sampler, float lod)
{
	RemapUV(uv.r, sampler->pSamplerState.AddressU);
	RemapUV(uv.t, sampler->pSamplerState.AddressV);

	if (uv.r < 0 || uv.t < 0)
	{
		unsigned char *color = sampler->pSamplerState.borderColor;
		return glm::vec4(color[0], color[1], color[2], color[3]);
	}
	float gx = uv.x * (width - 1);
	float gy = uv.y * (height - 1);
	int x = uv.x * (width - 1);
	int y = uv.y * (height - 1);
	int oldx = x;
	int oldy = y;
	int w = width;

	int index = 0;
	for (int i = 0; i < floor(lod); i++)
	{
		index += ByteWidth / std::pow(4, i);
		x >>= 1;
		y >>= 1;
		w >>= 1;
	}
	int maxIndex = index + ByteWidth / std::pow(4, floor(lod));
	int index0 = index +(x + y * w) * channels;

	switch (sampler->pSamplerState.Filter)
	{
	case FILTER_POINT_MIP_POINT:
	{
		return glm::vec4(m_Buffer[index0], m_Buffer[channels > 1 ? index0 + 1 : index0],
			m_Buffer[channels > 2 ? index0 + 2 : index0], channels > 3 ? m_Buffer[index0 + 3] : 255);
	}
	case FILTER_LINEAR_MIP_POINT:
	{
		const glm::vec4 cl00 = glm::vec4(m_Buffer[index0], m_Buffer[channels > 1 ? index0 + 1 : index0],
			m_Buffer[channels > 2 ? index0 + 2 : index0], channels > 3 ? m_Buffer[index0 + 3] : 255);

		int index1 = std::min(maxIndex, index + (x + y * w + 1) * channels);
		const glm::vec4 cl10 = glm::vec4(m_Buffer[index1], m_Buffer[channels > 1 ? index1 + 1 : index1],
			m_Buffer[channels > 2 ? index1 + 2 : index1], channels > 3 ? m_Buffer[index1 + 3] : 255);

		int index2 = std::min(maxIndex, index + (x + (y + 1) * w) * channels);
		const glm::vec4 cl01 = glm::vec4(m_Buffer[index2], m_Buffer[channels > 1 ? index2 + 1 : index2],
			m_Buffer[channels > 2 ? index2 + 2 : index2], channels > 3 ? m_Buffer[index2 + 3] : 255);

		int index3 = std::min(maxIndex, index + (x + 1 + (y + 1) * w) * channels);
		const glm::vec4 cl11 = glm::vec4(m_Buffer[index3], m_Buffer[channels > 1 ? index3 + 1 : index3],
			m_Buffer[channels > 2 ? index3 + 2 : index3], channels > 3 ? m_Buffer[index3 + 3] : 255);
		return Bilinear(gx - oldx, gy - oldy, cl00, cl10, cl01, cl11);
	}
	case FILTER_LINEAR_MIP_LINEAR:
	{
		const glm::vec4 cl00 = glm::vec4(m_Buffer[index0], m_Buffer[channels > 1 ? index0 + 1 : index0],
			m_Buffer[channels > 2 ? index0 + 2 : index0], channels > 3 ? m_Buffer[index0 + 3] : 255);

		int index1 = std::min(maxIndex, index + (x + y * w + 1) * channels);
		const glm::vec4 cl10 = glm::vec4(m_Buffer[index1], m_Buffer[channels > 1 ? index1 + 1 : index1],
			m_Buffer[channels > 2 ? index1 + 2 : index1], channels > 3 ? m_Buffer[index1 + 3] : 255);
		
		int index2 = std::min(maxIndex, index + (x + (y + 1) * w) * channels);
		const glm::vec4 cl01 = glm::vec4(m_Buffer[index2], m_Buffer[channels > 1 ? index2 + 1 : index2],
			m_Buffer[channels > 2 ? index2 + 2 : index2], channels > 3 ? m_Buffer[index2 + 3] : 255);

		int index3 = std::min(maxIndex, index + (x + 1 + (y + 1) * w) * channels);
		const glm::vec4 cl11 = glm::vec4(m_Buffer[index3], m_Buffer[channels > 1 ? index3 + 1 : index3],
			m_Buffer[channels > 2 ? index3 + 2 : index3], channels > 3 ? m_Buffer[index3 + 3] : 255);
		
		index += ByteWidth / std::pow(4, floor(lod));
		maxIndex = index + ByteWidth / std::pow(4, floor(lod + 1));
		x >>= 1;
		y >>= 1;
		w >>= 1;
 		index0 = index + (x + y * w) * channels;
		const glm::vec4 ch00 = glm::vec4(m_Buffer[index0], m_Buffer[channels > 1 ? index0 + 1 : index0],
			m_Buffer[channels > 2 ? index0 + 2 : index0], channels > 3 ? m_Buffer[index0 + 3] : 255);
		
		index1 = std::min(maxIndex, index + (x + y * w + 1) * channels);
		const glm::vec4 ch10 = glm::vec4(m_Buffer[index1], m_Buffer[channels > 1 ? index1 + 1 : index1],
			m_Buffer[channels > 2 ? index1 + 2 : index1], channels > 3 ? m_Buffer[index1 + 3] : 255);

		index2 = std::min(maxIndex, index + (x + (y + 1) * w) * channels);
		const glm::vec4 ch01 = glm::vec4(m_Buffer[index2], m_Buffer[channels > 1 ? index2 + 1 : index2],
			m_Buffer[channels > 2 ? index2 + 2 : index2], channels > 3 ? m_Buffer[index2 + 3] : 255);

		index3 = std::min(maxIndex, index + (x + 1 + (y + 1) * w) * channels);
		const glm::vec4 ch11 = glm::vec4(m_Buffer[index3], m_Buffer[channels > 1 ? index3 + 1 : index3],
			m_Buffer[channels > 2 ? index3 + 2 : index3], channels > 3 ? m_Buffer[index3 + 3] : 255);

		return (floor(lod + 1) - lod) * Bilinear(gx - oldx, gy - oldy, cl00, cl10, cl01, cl11) + (lod - floor(lod)) * Bilinear(gx - oldx, gy - oldy, ch00, ch10, ch01, ch11);
	}
	}
	return glm::vec4(255, 255, 255, 255);
}

glm::vec4 SRTexture2D::Sampler(glm::vec2 uv, SRSamplerState *sampler)
{
	return Sampler(uv, sampler, 0);
}

glm::vec4 SRTexture2D::Sampler(glm::vec2 uv, SRSamplerState *sampler, glm::vec2 ddx, glm::vec2 ddy)
{
	if (mipLevels > 1)
	{
		//Calculate lod level
		glm::vec2 dfdx = ddx * glm::vec2(width, height);
		glm::vec2 dfdy = ddy * glm::vec2(width, height);
		float L = std::max(glm::dot(dfdx, dfdx), glm::dot(dfdy, dfdy));
		return Sampler(uv, sampler, std::max(0.5f * log2(L), 0.0f));
	}
	return Sampler(uv, sampler);
}
