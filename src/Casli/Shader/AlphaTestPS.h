#pragma once
#include <algorithm>
#include "SRIPixelShader.h"

struct PixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 WorldPos;
};

struct AlphaTestBuffer
{
	glm::vec3 viewPos;
};

struct Directional
{
	glm::vec3 color;
	glm::vec3 dir;
};

struct Point
{
	glm::vec3 color;
	glm::vec3 pos;
	float Constant;
	float Linear;
	float Exp;
};

struct Spot
{
	glm::vec3 color;
	glm::vec3 pos;
	glm::vec3 dir;
	float Constant;
	float Linear;
	float Exp;
	float Cutoff;
};

struct RENDERER_API AlphaTestPS : public SRIPixelShader
{
	AlphaTestPS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "SV_Normal", 4, 12, 16 };
		Description sv_uv = { "SV_TEXCOORD0", 4, 8, 28 };
		Description sv_worldpos = { "SV_TEXCOORD1", 4, 16, 36 };
		inDesc.push_back(sv_position);
		inDesc.push_back(sv_normal);
		inDesc.push_back(sv_uv);
		inDesc.push_back(sv_worldpos);
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		PixelInput *input = (PixelInput *)in;
		//Point *point = (Point *)cbuffer;
		//glm::vec3 viewDir = *(glm::vec3 *)(cbuffer + sizeof(Point));
		//glm::vec4 diffuseColor = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());
		//
		//glm::vec3 lightDir = glm::vec4(point->pos, 1.0) - input->WorldPos;
		//glm::vec3 halfwayDir = glm::normalize(viewDir + lightDir);

		//float diff = glm::max(glm::dot(halfwayDir, input->normal), 0.0f);
		//float spec = glm::pow(glm::max(glm::dot(halfwayDir, input->normal), 0.0f), 1.0);
		color = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());
		if (color.w == 0) return true;

		return false;
	}
};