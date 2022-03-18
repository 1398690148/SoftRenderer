#pragma once
#include <algorithm>
#include "SRIVertexShader.h"

struct LambertVertexInput
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct LambertVertexOutput
{
	glm::vec4 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 WorldPos;
};

struct LambertCBuffer
{
	glm::mat4 ModelViewProjection;
	glm::mat4 Model;
	glm::mat4 ModelT;
};

class LambertVS : public SRIVertexShader
{
public:
	LambertVS()
	{
		Description position = { "Position", 4, 12, 0 };
		Description normal = { "Normal", 4, 12, 12 };
		Description uv = { "TEXCOORD0", 4, 8, 24 };
		inDesc.push_back(position);
		inDesc.push_back(normal);
		inDesc.push_back(uv);

		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "SV_Normal", 4, 12, 16 };
		Description sv_uv = { "SV_TEXCOORD0", 4, 8, 28 };
		Description sv_worldposition = { "SV_TEXCOORD1", 4, 16, 36 };
		outDesc.push_back(sv_position);
		outDesc.push_back(sv_normal);
		outDesc.push_back(sv_uv);
		outDesc.push_back(sv_worldposition);
	}

	unsigned char *vertex(unsigned char *input)
	{
		LambertVertexInput *in = (LambertVertexInput *)input;
		LambertCBuffer *cbuf = (LambertCBuffer *)cbuffer;
		LambertVertexOutput *o = new LambertVertexOutput();
		o->WorldPos = cbuf->Model * glm::vec4(in->pos, 1.0f);
		o->pos = cbuf->ModelViewProjection * glm::vec4(in->pos, 1.0f);
		o->normal = glm::vec3(0, 0, 0);// cbuf->ModelT * glm::vec4(in->normal, 1.0);
		o->uv = in->uv;
		return (unsigned char *)o;
	}
};