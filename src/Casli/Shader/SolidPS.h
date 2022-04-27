#pragma once
#include <algorithm>
#include "SRIPixelShader.h"

struct SolidPixelInput
{
	glm::vec4 position;
	glm::vec4 color;
};

//struct SolidPixelCBuffer
//{
//	glm::vec3 viewPos;
//};


struct SolidPS : public SRIPixelShader
{
	SolidPS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_color = { "Color", 4, 16, 16 };
		inDesc.push_back(sv_position);
		inDesc.push_back(sv_color);
		//cbuffer = new unsigned char[sizeof(SolidPixelCBuffer)];
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		SolidPixelInput *input = (SolidPixelInput *)in;
		color = input->color * 255.f;
		return false;
	}
};