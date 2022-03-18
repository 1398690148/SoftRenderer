#pragma once
#include <algorithm>
#include "SRIPixelShader.h"

struct SolidPixelInput
{
	glm::vec4 position;
};

struct SolidPixelCBuffer
{
	glm::vec3 viewPos;
};


struct SolidPS : public SRIPixelShader
{
	SolidPS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		inDesc.push_back(sv_position);
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		color = glm::vec4(255, 255, 255, 255);
		return false;
	}
};