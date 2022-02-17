#pragma once
#include <algorithm>
#include "IVertexShader.h"


struct VertexInput
{
	Vec4f pos;
	Vec4f color;
	Vec2f uv;
};

struct Output
{
	Vec4f pos;
	Vec4f color;
	Vec2f uv;
};

struct cbuffer
{
	Matrix ModelViewProjection;
};

class LambertVertexShader : public IVertexShader
{
public:
	LambertVertexShader()
	{
		Description position = { "Position", 4, 16, 0 };
		Description color = { "Color", 4, 16, 16 };
		Description uv = { "UV", 4, 8, 32 };
		inDesc.push_back(position);
		inDesc.push_back(color);
		inDesc.push_back(uv);

		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_color = { "Color", 4, 16, 16 };
		Description sv_uv = { "UV", 4, 8, 32 };
		outDesc.push_back(sv_position);
		outDesc.push_back(sv_color);
		outDesc.push_back(sv_uv);
	}

	unsigned char *vertex(unsigned char *input)
	{
		VertexInput *in = (VertexInput *)input;
		cbuffer *cbuf = (cbuffer *)buffer;
		Output *o = new Output();
		o->pos = cbuf->ModelViewProjection * in->pos;
		o->color = in->color;
		o->uv = in->uv;
		return (unsigned char *)o;
	}
};