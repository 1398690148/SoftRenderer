#pragma once
#include "SRShaderInput.h"

class CORE_API SRIVertexShader
{
public:
	SRIVertexShader();
	virtual unsigned char *vertex(unsigned char *input) = 0;
	virtual ~SRIVertexShader();
	unsigned char *cbuffer{};
	std::vector<Description> inDesc;
	std::vector<Description> outDesc;
	SRTexture2D **textures{};
	SRSamplerState **samplers{};
};