#pragma once
#include <algorithm>
#include "SRIPixelShader.h"

struct LambertPixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 WorldPos;
};

struct LambertDirectional
{
	glm::vec3 viewPos;
	glm::vec3 color;
	glm::vec3 dir;
};

struct LambertPS : public SRIPixelShader
{
	LambertPS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "SV_Normal", 4, 12, 16 };
		Description sv_uv = { "SV_TEXCOORD0", 4, 8, 28 };
		Description sv_worldpos = { "SV_TEXCOORD1", 4, 16, 36 };
		inDesc.push_back(sv_position);
		inDesc.push_back(sv_normal);
		inDesc.push_back(sv_uv);
		inDesc.push_back(sv_worldpos);
		cbuffer = new unsigned char[sizeof(LambertDirectional)];
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		PixelInput *input = (PixelInput *)in;
		LambertDirectional *directional = (LambertDirectional *)cbuffer;
		glm::vec3 normal = glm::normalize(input->normal);
		glm::vec4 diffuseColor = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());
		glm::vec4 ambient = diffuseColor * 0.3f;

		//Directional Light
		float diff = max(glm::dot(normal, glm::normalize(directional->dir)), 0.0f);
		color = diffuseColor * glm::vec4(directional->color, 1.0) * diff + ambient;
		color = glm::vec4(min(255.0f, color.r), min(255.0f, color.g), min(255.0f, color.b), color.a);
		return false;
	}
};