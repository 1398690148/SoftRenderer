#pragma once
#include "ShaderInput.h"
#include <tbb/enumerable_thread_specific.h>

class CORE_API IPixelShader
{
public:
	IPixelShader();
	virtual bool fragment(unsigned char *in, glm::vec4 &color) = 0;
	virtual ~IPixelShader();
	std::vector<Description> inDesc;
	unsigned char *cbuffer{};
	Texture2D **textures{};
	SamplerState **samplers{};
	tbb::enumerable_thread_specific<glm::vec2> dFdx;
	tbb::enumerable_thread_specific<glm::vec2> dFdy;
};

