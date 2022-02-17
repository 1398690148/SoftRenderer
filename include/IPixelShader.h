#pragma once
#include "CoreAPI.h"
#include "Math/Math.h"
#include "Texture2D.h"
#include "SamplerState.h"

class CORE_API IPixelShader
{
public:
	IPixelShader();
	virtual bool fragment(unsigned char *in, Vec4f &color) = 0;
	virtual ~IPixelShader();
	unsigned char *buffer{};
	Texture2D **textures{};
	SamplerState **samplers{};
};