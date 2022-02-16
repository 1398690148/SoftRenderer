#pragma once
#include <algorithm>
#include "IVertexShader.h"

struct Input : public InputData
{
	float x;
	float y;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct Output : public OutputData
{
	float x;
	float y;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct LambertVertexShader : public IVertexShader
{
	LambertVertexShader()
	{

	}

	virtual OutputData *vertex(InputData *input)
	{
		Output *o = new Output();
		Input *in = dynamic_cast<Input *>(input);
		o->x = in->x;
		o->y = in->y;
		o->r = in->r;
		o->g = in->g;
		o->b = in->b;
		o->a = in->a;
		return o;
	}
	virtual bool fragment(Vec3f bar, TGAColor &color)
	{
		Vec2f uv = varying_uv * bar;
		Vec3f n = proj<3>(uniform_MIT * embed<4>(model->normal(uv))).normalize();
		Vec3f l = proj<3>(uniform_M * embed<4>(light_dir)).normalize();
		float intensity = std::max(0.f, n * l);
		color = model->diffuse(uv) * intensity;
		return false;
	}
};