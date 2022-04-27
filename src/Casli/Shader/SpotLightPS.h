#pragma once
#include <algorithm>
#include <glm/gtx/transform.hpp>
#include "SRIPixelShader.h"
using namespace glm;

struct SpotLightPixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 WorldPos;
};

struct SpotCBuf
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 dir;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLightPS : public SRIPixelShader
{
	SpotLightPS()
	{
		Description sv_position = { "SV_Position", 4, 16, 0 };
		Description sv_normal = { "SV_Normal", 4, 12, 16 };
		Description sv_uv = { "SV_TEXCOORD0", 4, 8, 28 };
		Description sv_worldpos = { "SV_TEXCOORD1", 4, 16, 36 };
		inDesc.push_back(sv_position);
		inDesc.push_back(sv_normal);
		inDesc.push_back(sv_uv);
		inDesc.push_back(sv_worldpos);
		cbuffer = new unsigned char[sizeof(SpotCBuf)];
	}
	float clamp(float t, float min, float max)
	{
		if (t > max) return max;
		if (t < min) return min;
		return t;
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		SpotLightPixelInput *input = (SpotLightPixelInput *)in;
		SpotCBuf *spot = (SpotCBuf *)(cbuffer);

		vec3 ambient = vec3(textures[0]->Sampler(input->uv, samplers[0])) * 0.3f;
		vec3 lightDir = (vec3(input->WorldPos) - spot->pos);
		float theta = dot(normalize(spot->dir), normalize(lightDir));
		float epsilon = spot->cutOff - spot->outerCutOff;
		float intensity = clamp((theta - spot->outerCutOff) / epsilon, 0.0, 1.0);

		vec3 diffuse = spot->color * vec3(textures[0]->Sampler(input->uv, samplers[0]));
		// attenuation
		float distance = length(spot->pos - vec3(input->WorldPos));
		float attenuation = 1.0 / (spot->constant + spot->linear * distance + spot->quadratic * (distance * distance));
		diffuse *= intensity;
		diffuse *= attenuation;
		color = vec4(diffuse, 255);
		color += vec4(ambient, 0.0);
		color = glm::vec4(min(255.0f, color.r), min(255.0f, color.g), min(255.0f, color.b), color.a);

		return false;
	}
};