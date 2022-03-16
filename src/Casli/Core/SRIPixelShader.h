#pragma once
#include "SRShaderInput.h"
#include <tbb/enumerable_thread_specific.h>

class CORE_API SRIPixelShader
{
public:
	SRIPixelShader();
	virtual bool fragment(unsigned char *in, glm::vec4 &color) = 0;
	virtual ~SRIPixelShader();
	std::vector<Description> inDesc;
	unsigned char *cbuffer{};
	SRTexture2D **textures{};
	SRSamplerState **samplers{};
	tbb::enumerable_thread_specific<glm::vec2> dFdx;
	tbb::enumerable_thread_specific<glm::vec2> dFdy;
};

