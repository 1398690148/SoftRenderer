#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"
#include "Math/Math.h"

class SamplerState;

class CORE_API Texture2D : public IBuffer
{
public:
	Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd);
	Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd, IBuffer *pDepthBuffer);
	~Texture2D();

	void GenerateMips();
	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }
	Vec4f Sampler(Vec2f uv, SamplerState *sampler);
private:
	Vec4f Bilinear(const float &tx, const float  &ty, const Vec4f &c00, const Vec4f &c10, const Vec4f &c01, const Vec4f &c11);
private:
	unsigned int width;
	unsigned int height;
	unsigned int mipLevels;
	DataFormat format;
	unsigned int SysMemPitch;
	int channels;
};