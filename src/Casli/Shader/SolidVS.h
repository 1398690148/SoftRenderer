#pragma once
#include <algorithm>
#include "SRIVertexShader.h"

struct SolidVertexInput
{
	glm::vec3 pos;
};

struct SolidVertexOutput
{
	glm::vec4 pos;
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
		inDesc.push_back(position);

		Description sv_position = { "SV_Position", 4, 16, 0 };
		outDesc.push_back(sv_position);
	}

	unsigned char *vertex(unsigned char *input)
	{
		SolidVertexInput *in = (SolidVertexInput *)input;
		SolidVertexCBuffer *cbuf = (SolidVertexCBuffer *)cbuffer;
		SolidVertexOutput *o = new SolidVertexOutput();
		o->pos = cbuf->ModelViewProjection * glm::vec4(in->pos, 1.0f);
		return (unsigned char *)o;
	}
};