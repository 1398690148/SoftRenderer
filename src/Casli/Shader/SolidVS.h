#pragma once
#include <algorithm>
#include "SRIVertexShader.h"

struct SolidVertexInput
{
	glm::vec3 pos;
	glm::vec3 color;
};

struct SolidVertexOutput
{
	glm::vec4 pos;
	glm::vec4 color;
};

struct SolidVertexCBuffer
{
	glm::mat4 ModelViewProjection;
};

class SolidVS : public SRIVertexShader
{
public:
	SolidVS()
	{
		Description position = { "Position", 4, 12, 0 };
		Description color = { "Color", 4, 12, 12 };
		inDesc.push_back(position);
		inDesc.push_back(color);

		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_color = { "Color", 4, 16, 16 };
		outDesc.push_back(sv_position);
		outDesc.push_back(sv_color);
	}

	unsigned char *vertex(unsigned char *input)
	{
		SolidVertexInput *in = (SolidVertexInput *)input;
		SolidVertexCBuffer *cbuf = (SolidVertexCBuffer *)cbuffer;
		SolidVertexOutput *o = new SolidVertexOutput();
		o->pos = cbuf->ModelViewProjection * glm::vec4(in->pos, 1.0f);
		o->color = glm::vec4(in->color, 1.0);
		return (unsigned char *)o;
	}
};