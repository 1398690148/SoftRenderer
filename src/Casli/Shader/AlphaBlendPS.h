#pragma once
#include <algorithm>
#include "IPixelShader.h"
#include "../Renderer/RendererAPI.h"

struct AlphaPixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct RENDERER_API AlphaBlendPS : public IPixelShader
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
		color = glm::vec4(255, 0, 0, 127);
		return false;
	}
};