#pragma once
#include "ShaderInput.h"

class CORE_API IPixelShader
{
public:
	IPixelShader();
	virtual bool fragment(unsigned char *in, glm::vec4 &color) = 0;
	virtual ~IPixelShader();
	std::vector<Description> inDesc;
	unsigned char *buffer{};
	Texture2D **textures{};
	SamplerState **samplers{};
	glm::vec2 ddx;
	glm::vec2 ddy;
};

