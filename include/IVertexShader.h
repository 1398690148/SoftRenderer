#pragma once
#include "ShaderInput.h"

class CORE_API IVertexShader
{
public:
	IVertexShader();
	virtual unsigned char *vertex(unsigned char *input) = 0;
	virtual ~IVertexShader();
	unsigned char *cbuffer{};
	std::vector<Description> inDesc;
	std::vector<Description> outDesc;
	Texture2D **textures{};
	SamplerState **samplers{};
};