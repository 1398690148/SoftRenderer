#pragma once
#include "SRCore.h"
#include "SRCoreAPI.h"
#include "SRIBuffer.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class SRSamplerState;

class CORE_API SRTexture2D : public SRIBuffer
{
public:
	SRTexture2D(TEXTURE2D_DESC *desc);
	SRTexture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd);
	~SRTexture2D();

	void GenerateMips();
	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }

	unsigned char GetDepth(int u, int v) 
	{ 
		unsigned int offset = 4 * (u * width + v);
		return *GetBuffer(offset); 
	}

	glm::vec4 Sampler(glm::vec2 uv, SRSamplerState *sampler);
	glm::vec4 Sampler(glm::vec2 uv, SRSamplerState *sampler, glm::vec2 ddx, glm::vec2 ddy);

private:
	glm::vec4 Bilinear(const float &tx, const float  &ty, const glm::vec4 &c00, const glm::vec4 &c10, const glm::vec4 &c01, const glm::vec4 &c11);
	void RemapUV(float &texCoord, TEXTURE_ADDRESS_MODE address);
	glm::vec4 Sampler(glm::vec2 uv, SRSamplerState *sampler, float lod);
private:
	int width;
	int height;
	int channels;
	unsigned int mipLevels;
};