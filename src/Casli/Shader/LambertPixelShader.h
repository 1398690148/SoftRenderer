#pragma once
#include <algorithm>
#include "IPixelShader.h"

struct PixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 WorldPos;
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

struct LambertPixelShader : public IPixelShader
{
	LambertPixelShader()
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
		Directional *directional = (Directional *)cbuffer;
		Point *point = (Point *)(cbuffer + sizeof(Directional));
		Spot *spot = (Spot *)(cbuffer + sizeof(Directional) + sizeof(Point));
		glm::vec3 normal = glm::normalize(input->normal);

		//Directional Light
		glm::vec4 directionalLight = std::max(glm::dot(normal, directional->dir), 0.0f) * glm::vec4(directional->color, 1.0);
		color += directionalLight;

		//Point Light
		float distance = glm::length(point->pos - glm::vec3(input->WorldPos));
		float attenuation = 1.0 / (point->Constant + point->Linear * distance + (distance * distance) * point->Exp);
		color += glm::vec4(point->color, 1.0) * attenuation;

		//Spot Light
		glm::vec3 vertexDir = glm::normalize(glm::vec3(input->WorldPos) - spot->pos);
		float spotFactor = glm::dot(vertexDir, spot->dir);
		if (spotFactor > spot->Cutoff)
		{
			distance = glm::length(spot->pos - glm::vec3(input->WorldPos));
			attenuation = 1.0 / (spot->Constant + spot->Linear * distance + (distance * distance) * spot->Exp);
			color += glm::vec4(spot->color, 1.0) * (1.0f - (1.0f - spotFactor) * 1.0f / (1.0f - spot->Cutoff));
		}
		for (int i = 0; i < 4; i++)
		{
			color[i] = std::min(color[i], 1.0f);
		}
		
		color = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local()) * color;
		return false;
	}
};