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

struct BlinnPhongCBuffer
{
	glm::vec3 viewPos;
};

struct Directional
{
	glm::vec3 color;
	glm::vec3 dir;
};

struct BlinnPhongPS : public SRIPixelShader
{
	BlinnPhongPS()
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
		BlinnPhongCBuffer *buffer = (BlinnPhongCBuffer *)(cbuffer + sizeof(Directional));
		Directional *directional = (Directional *)cbuffer;
		glm::vec3 normal = glm::normalize(input->normal);

		glm::vec4 diff = textures[0]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());
		glm::vec4 spec(0, 0, 0, 0);
		if (textures[1])
			spec = textures[1]->Sampler(input->uv, samplers[0], dFdx.local(), dFdy.local());

		glm::vec3 viewDir = glm::normalize(buffer->viewPos - glm::vec3(input->WorldPos));
		glm::vec3 halfwayDir = glm::normalize(viewDir + glm::normalize(directional->dir));
		float diffCoe = std::max(glm::dot(normal, directional->dir), 0.0f);
		float specCoe = glm::pow(glm::max(glm::dot(halfwayDir, normal), 0.0f), 5.0);

		glm::vec4 ambientColor = diff * 0.2f * glm::vec4(directional->color, 1.0);
		glm::vec4 diffuseColor = diff * diffCoe * glm::vec4(directional->color, 1.0);
		glm::vec4 specColor = spec * specCoe * glm::vec4(directional->color, 1.0);
		color = ambientColor + diffuseColor + specColor;
		for (int i = 0; i < 4; i++)
		{
			color[i] = std::min(255.0f, color[i]);
		}
		return false;
	}
};