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

struct Point
{
	glm::vec3 color;
	glm::vec3 pos;
	float Constant;
	float Linear;
	float Exp;
};

struct PointPixelShader : public IPixelShader
{
	PointPixelShader()
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
		Point *point = (Point *)(cbuffer);
		glm::vec3 viewPos = *(glm::vec3 *)(cbuffer + sizeof(Point) * 2);
		glm::vec3 normal = glm::normalize(input->normal);

		//Point Light
		float distance = glm::length(point[0].pos - glm::vec3(input->WorldPos));
		float attenuation = 1.0 / (point[0].Constant + point[0].Linear * distance + (distance * distance) * point[0].Exp);
		color += glm::vec4(point[0].color, 1.0) * attenuation;

		distance = glm::length(point[1].pos - glm::vec3(input->WorldPos));
		attenuation = 1.0 / (point[1].Constant + point[1].Linear * distance + (distance * distance) * point[1].Exp);
		color += glm::vec4(point[1].color, 1.0) * attenuation;

		distance = glm::length(point[2].pos - glm::vec3(input->WorldPos));
		attenuation = 1.0 / (point[2].Constant + point[2].Linear * distance + (distance * distance) * point[2].Exp);
		color += glm::vec4(point[2].color, 1.0) * attenuation;

		color = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local()) * color;
		return false;
	}
};