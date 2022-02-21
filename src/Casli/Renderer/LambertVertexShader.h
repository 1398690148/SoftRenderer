#pragma once
#include <algorithm>
#include "IVertexShader.h"

struct VertexInput
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Output
{
	glm::vec4 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct cbuffer
{
	glm::mat4 ModelViewProjection;
};

class LambertVertexShader : public IVertexShader
{
public:
	LambertVertexShader()
	{
		Description position = { "Position", 4, 12, 0 };
		Description normal = { "Normal", 4, 12, 12 };
		Description uv = { "UV", 4, 8, 24 };
		inDesc.push_back(position);
		inDesc.push_back(normal);
		inDesc.push_back(uv);

		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "Normal", 4, 12, 16 };
		Description sv_uv = { "UV", 4, 8, 28 };
		outDesc.push_back(sv_position);
		outDesc.push_back(sv_normal);
		outDesc.push_back(sv_uv);
	}

	unsigned char *vertex(unsigned char *input)
	{
		VertexInput *in = (VertexInput *)input;
		cbuffer *cbuf = (cbuffer *)buffer;
		Output *o = new Output();
		o->pos = cbuf->ModelViewProjection * glm::vec4(in->pos, 1.0f);
		o->normal = in->normal;
		o->uv = in->uv;
		return (unsigned char *)o;
	}
};