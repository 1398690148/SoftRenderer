#pragma once
#include <algorithm>
#include "IPixelShader.h"

struct Input : public InputData
{
	float x;
	float y;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct LambertPixelShader : public IPixelShader
{
	LambertPixelShader()
	{

	}
	virtual bool fragment(InputData *in, Vec4f &color)
	{
		color[0] = 1.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		color[3] = 1.0f;
		return false;
	}
};