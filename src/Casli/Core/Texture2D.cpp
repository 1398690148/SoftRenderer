#include "Texture2D.h"
#include "SamplerState.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <cmath>
#include "tgaimage.h"

Texture2D::Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd)
	: IBuffer(sd ? sd->SysMemPitch * desc->Height : desc->Width * desc->Height * 4, sizeof(unsigned char)), width(desc->Width), height(desc->Height),
	mipLevels(desc->MipLevels), format(desc->Format)
{
	if (mipLevels > 1)
	{
		m_Buffer = new unsigned char[ByteWidth * 4 / 3];
		memset(m_Buffer, 0, ByteWidth + ByteWidth / 3);
	}
	else
	{
		m_Buffer = new unsigned char[ByteWidth];
	}
	if (sd)
	{
		memcpy(m_Buffer, sd->pSysMem, ByteWidth);
		SysMemPitch = sd->SysMemPitch;

	}
	channels = SysMemPitch / width;
}

Texture2D::~Texture2D()
{
}

void Texture2D::GenerateMips()
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
				glm::vec4 c00 = glm::vec4(m_Buffer[index0], m_Buffer[index0 + 1], m_Buffer[index0 + 2], 255);
				glm::vec4 c01 = glm::vec4(m_Buffer[index1], m_Buffer[index1 + 1], m_Buffer[index1 + 2], 255);
				glm::vec4 c10 = glm::vec4(m_Buffer[index2], m_Buffer[index2 + 1], m_Buffer[index2 + 2], 255);
				glm::vec4 c11 = glm::vec4(m_Buffer[index3], m_Buffer[index3 + 1], m_Buffer[index3 + 2], 255);
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
				//TGAColor color((c00.r + c01.r + c10.r + c11.r) / 4, (c00.g + c01.g + c10.g + c11.g) / 4, 
				//	(c00.b + c01.b + c10.b + c11.b) / 4, (c00.a + c01.a + c10.a + c11.a) / 4);
				//image.set(i / 2, j / 2, color);
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
	//image.flip_vertically();
	//image.write_tga_file("../output.tga");
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

glm::vec4 Texture2D::Sampler(glm::vec2 uv, SamplerState * sampler, int lod)
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
	
	int index = 0;
	for (int i = 0; i < lod; i++)
	{
		index += ByteWidth / std::pow(4, i);
		x >>= 1;
		y >>= 2;
	}

	switch (channels)
	{
	case 1:
	{
		if (sampler->pSamplerState.Filter == FILTER_POINT)
		{
			index = index + (x + y * width);
			return glm::vec4(m_Buffer[index], m_Buffer[index], m_Buffer[index], 1);
		}
		else if (sampler->pSamplerState.Filter == FILTER_LINEAR)
		{
			int index0 = index + (x + y * width);
			unsigned int maxIndex = 4 * (width - 1) * (height - 1) - 4;
			const glm::vec4 c00 = glm::vec4(m_Buffer[index0], m_Buffer[index0], m_Buffer[index0], 255);
			int index1 = std::min(maxIndex, index + (x + y * width + 1));
			const glm::vec4 c10 = glm::vec4(m_Buffer[index1], m_Buffer[index1], m_Buffer[index1], 255);
			int index2 = std::min(maxIndex, index + (x + (y + 1) * width));
			const glm::vec4 c01 = glm::vec4(m_Buffer[index2], m_Buffer[index2], m_Buffer[index2], 255);
			int index3 = std::min(maxIndex, index + (x + 1 + (y + 1) * width));
			const glm::vec4 c11 = glm::vec4(m_Buffer[index3], m_Buffer[index3], m_Buffer[index3], 255);
			return Bilinear(gx - x, gy - y, c00, c10, c01, c11);
		}
	}
	break;
	case 3:
	{
		if (sampler->pSamplerState.Filter == FILTER_POINT)
		{
			index = index + (x + y * width) * 3;
			return glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], 1);
		}
		else if (sampler->pSamplerState.Filter == FILTER_LINEAR)
		{
			int index0 = index + (x + y * width) * 3;
			unsigned int maxIndex = 3 * (width - 1) * (height - 1) - 3;
			const glm::vec4 c00 = glm::vec4(m_Buffer[index0], m_Buffer[index0 + 1], m_Buffer[index0 + 2], 255);
			int index1 = std::min(maxIndex, (x + y * width + 1));
			const glm::vec4 c10 = glm::vec4(m_Buffer[index1], m_Buffer[index1 + 1], m_Buffer[index1 + 2], 255);
			int index2 = std::min(maxIndex, (x + (y + 1) * width));
			const glm::vec4 c01 = glm::vec4(m_Buffer[index2], m_Buffer[index2 + 1], m_Buffer[index2 + 2], 255);
			int index3 = std::min(maxIndex, (x + 1 + (y + 1) * width));
			const glm::vec4 c11 = glm::vec4(m_Buffer[index3], m_Buffer[index3 + 1], m_Buffer[index3 + 2], 255);
			return Bilinear(gx - x, gy - y, c00, c10, c01, c11);
		}
	}
	break;
	case 4:
	{
		if (sampler->pSamplerState.Filter == FILTER_POINT)
		{
			index = index + (x + y * width) * 4;
			return glm::vec4(m_Buffer[index], m_Buffer[index + 1], m_Buffer[index + 2], m_Buffer[index + 3]);
		}
		else if (sampler->pSamplerState.Filter == FILTER_LINEAR)
		{
			int index0 = index + (x + y * width) * 4;
			unsigned int maxIndex = 4 * (width - 1) * (height - 1) - 4;
			const glm::vec4 c00 = glm::vec4(m_Buffer[index0], m_Buffer[index0 + 1], m_Buffer[index0 + 2], m_Buffer[index0 + 3]);
			int index1 = std::min(maxIndex, (x + y * width + 1) * 4);
			const glm::vec4 c10 = glm::vec4(m_Buffer[index1], m_Buffer[index1 + 1], m_Buffer[index1 + 2], m_Buffer[index1 + 3]);
			int index2 = std::min(maxIndex, (x + (y + 1) * width) * 4);
			const glm::vec4 c01 = glm::vec4(m_Buffer[index2], m_Buffer[index2 + 1], m_Buffer[index2 + 2], m_Buffer[index2 + 3]);
			int index3 = std::min(maxIndex, (x + 1 + (y + 1) * width) * 4);
			const glm::vec4 c11 = glm::vec4(m_Buffer[index3], m_Buffer[index3 + 1], m_Buffer[index3 + 2], m_Buffer[index3 + 3]);
			return Bilinear(uv.x, uv.y, c00, c10, c01, c11);
		}
	}
	break;
	}
	return glm::vec4(255, 255, 255, 255);
}

glm::vec4 Texture2D::Sampler(glm::vec2 uv, SamplerState *sampler)
{
	return Sampler(uv, sampler, 0);
}

glm::vec4 Texture2D::Sampler(glm::vec2 uv, SamplerState *sampler, glm::vec2 ddx, glm::vec2 ddy)
{
	if (mipLevels > 1)
	{
		//Calculate lod level
		glm::vec2 dfdx = ddx * glm::vec2(width, height);
		glm::vec2 dfdy = ddy * glm::vec2(width, height);
		float L = glm::max(glm::dot(dfdx, dfdx), glm::dot(dfdy, dfdy));
		return Sampler(uv, sampler, glm::max(0.5f * glm::log2(L), 0.0f));
	}
	return glm::vec4(255, 255, 255, 255);
}
