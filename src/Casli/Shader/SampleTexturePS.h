#pragma once
#include <algorithm>
#include "SRIPixelShader.h"

struct PixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct SampleTexturePS : public SRIPixelShader
{
	SampleTexturePS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "SV_Normal", 4, 12, 16 };
		Description sv_uv = { "SV_TEXCOORD0", 4, 8, 28 };
		inDesc.push_back(sv_position);
		inDesc.push_back(sv_normal);
		inDesc.push_back(sv_uv);
		//cbuffer = new unsigned char[100];
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		PixelInput *input = (PixelInput *)in;
		color = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());
		return false;
	}
};