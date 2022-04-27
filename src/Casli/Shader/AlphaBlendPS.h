#pragma once
#include <algorithm>
#include "SRIPixelShader.h"

struct AlphaPixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct AlphaBlendPS : public SRIPixelShader
{
	AlphaBlendPS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "SV_Normal", 4, 12, 16 };
		Description sv_uv = { "SV_TEXCOORD0", 4, 8, 28 };
		inDesc.push_back(sv_position);
		inDesc.push_back(sv_normal);
		inDesc.push_back(sv_uv);
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		AlphaPixelInput *input = (AlphaPixelInput *)in;
		color = glm::vec4(255.0, 0, 0, 210);
		return false;
	}
};