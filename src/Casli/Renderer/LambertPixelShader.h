#pragma once
#include <algorithm>
#include "IPixelShader.h"

struct PixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
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
		color = textures[0]->Sampler(input->uv, samplers[0]);//input->color;
		//color = glm::vec4(255, 255, 255, 255) * std::max(0.0f, glm::dot(input->normal, glm::vec3(1, 1, 1)));
		//if (textures[1] != nullptr)
		{
			//color *= textures[1]->Sampler(input->uv, samplers[0]);
		}
		return false;
	}
};