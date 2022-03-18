#pragma once
#include <algorithm>
#include "SRIPixelShader.h"

struct AlphaTestPixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 WorldPos;
};

struct AlphaTestPS : public SRIPixelShader
{
	AlphaTestPS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "SV_Normal", 4, 12, 16 };
		Description sv_uv = { "SV_TEXCOORD0", 4, 8, 28 };
		Description sv_worldpos = { "SV_TEXCOORD1", 4, 16, 36 };
		inDesc.push_back(sv_position);
		inDesc.push_back(sv_normal);
		inDesc.push_back(sv_uv);
		inDesc.push_back(sv_worldpos);
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		AlphaTestPixelInput *input = (AlphaTestPixelInput *)in;
		color = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());
		if (color.w == 0) return true;
		return false;
	}
};