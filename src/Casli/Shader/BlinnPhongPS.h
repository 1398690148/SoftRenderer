#pragma once
#include <algorithm>
#include "SRIPixelShader.h"
#include <glm/gtx/transform.hpp>

struct BlinnPhongPixelInput
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 WorldPos;
};

struct Directional
{
	glm::vec3 viewPos;
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
		cbuffer = new unsigned char[sizeof(Directional)];
	}
	virtual bool fragment(unsigned char *in, glm::vec4 &color)
	{
		BlinnPhongPixelInput *input = (BlinnPhongPixelInput *)in;
		Directional *directional = (Directional *)cbuffer;
		glm::vec3 normal = glm::normalize(input->normal);

		glm::vec3 diffuseColor = textures[0]->Sampler(input->uv, samplers[0]);
		glm::vec3 ambient = diffuseColor * 0.3f;

		float diff = std::max(dot(normal, glm::normalize(directional->dir)), 0.0f);
		diffuseColor *= diff;
		diffuseColor *= directional->color;

		glm::vec3 viewDir = glm::normalize(directional->viewPos - glm::vec3(input->WorldPos));
		glm::vec3 halfwayDir = glm::normalize(viewDir + directional->dir);
		float spec = glm::pow(std::max(glm::dot(halfwayDir, normal), 0.0f), 1.0);
		glm::vec3 specular = directional->color * spec * glm::vec3(textures[1]->Sampler(input->uv, samplers[0]));

		color = glm::vec4(ambient + diffuseColor + specular, 255);
		color = glm::vec4(min(255.0f, color.r), min(255.0f, color.g), min(255.0f, color.b), color.a);
		return false;
	}
};