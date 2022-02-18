#pragma once
#include <algorithm>
#include "IPixelShader.h"

struct PixelInput
{
	glm::vec4 position;
	//glm::vec4 color;
	glm::vec2 uv;
};

struct LambertPixelShader : public IPixelShader
{
	LambertPixelShader()
	{

	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		PixelInput *input = (PixelInput *)in;
		color = textures[0]->Sampler(input->uv, samplers[0]);
		return false;
	}
};