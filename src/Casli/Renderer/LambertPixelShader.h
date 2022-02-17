#pragma once
#include <algorithm>
#include "IPixelShader.h"

struct PixelInput
{
	Vec4f position;
	Vec4f color;
	Vec2f uv;
};

struct LambertPixelShader : public IPixelShader
{
	LambertPixelShader()
	{

	}
	virtual bool fragment(unsigned char *in, Vec4f &color)
	{
		PixelInput *input = (PixelInput *)in;
		color = textures[0]->Sampler(input->uv, samplers[0]);
		return false;
	}
};