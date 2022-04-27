#pragma once
#include "SRIPixelShader.h"

struct BoardPixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 WorldPos;
};

struct BoardDirectional
{
	glm::vec3 viewPos;
	glm::vec3 color;
	glm::vec3 dir;
};

struct BoardPS : public SRIPixelShader
{
	BoardPS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "SV_Normal", 4, 12, 16 };
		Description sv_uv = { "SV_TEXCOORD0", 4, 8, 28 };
		Description sv_worldpos = { "SV_TEXCOORD1", 4, 16, 36 };
		inDesc.push_back(sv_position);
		inDesc.push_back(sv_normal);
		inDesc.push_back(sv_uv);
		inDesc.push_back(sv_worldpos);
		cbuffer = new unsigned char[sizeof(BoardDirectional)];
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		BoardPixelInput *input = (BoardPixelInput *)in;
		BoardDirectional *directional = (BoardDirectional *)cbuffer;
		//glm::vec3 normal = glm::normalize(input->normal);

		glm::vec3 diffuseColor = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());

		//float diff = std::max(dot(normal, glm::normalize(directional->dir)), 0.0f);
		//diffuseColor *= diff;
		//diffuseColor *= directional->color;

		glm::vec4 diffuse1 = textures[3]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());
		color = glm::vec4(diffuseColor, 255) + diffuse1;
		return false;
	}
};