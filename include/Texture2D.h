#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"
#include "Math/Math.h"

class CORE_API Texture2D : public IBuffer
{
	friend class SamplerState;
public:
	Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd);
	Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd, IBuffer *pDepthBuffer);
	~Texture2D();

	void GenerateMips();
	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }
	Vec4f Sampler(Vec2f uv, SamplerState *sampler);
private:
	unsigned int width;
	unsigned int height;
	unsigned int mipLevels;
	DataFormat format;
	unsigned int SysMemPitch;
};