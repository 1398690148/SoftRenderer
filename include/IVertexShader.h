#pragma once
#include "ShaderInput.h"

class CORE_API IVertexShader
{
public:
	virtual unsigned char *vertex(unsigned char *input) = 0;
	virtual ~IVertexShader() { delete buffer; }
	unsigned char *buffer{};
	std::vector<Description> inDesc;
	std::vector<Description> outDesc;
	std::vector<Texture2D> textures;
	std::vector<SamplerState> samplers;
};