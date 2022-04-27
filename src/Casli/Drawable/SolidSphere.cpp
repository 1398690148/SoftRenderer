#include "SolidSphere.h"
#include "Bindable.h"
#include "Sphere.h"
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Texture.h>
#include <Sampler.h>
#include <Topology.h>
#include <InputLayout.h>
#include <SolidVS.h>
#include <SolidPS.h>
#include <VertexShader.h>
#include <PixelShader.h>
#include <TransformCbuf.h>
#include <Blender.h>
#include <glm/gtx/transform.hpp>

SolidSphere::SolidSphere(Graphics& gfx, float radius, glm::vec3 color)
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
	};
	Sphere *sphere = new Sphere();
	std::vector<glm::vec3> position = std::move(sphere->getVertices());
	std::vector<Vertex> vertex;
	for (auto iter : position)
	{
		vertex.push_back({ iter, color });
	}
	AddBind(std::make_shared<VertexBuffer>(gfx, vertex));
	AddBind(std::make_shared<IndexBuffer>(gfx, sphere->getIndices()));

	AddBind(std::make_unique<VertexShader>(gfx, new SolidVS()));
	AddBind(std::make_unique<PixelShader>(gfx, new SolidPS()));
	AddBind(std::make_unique<Texture>(gfx, "", 1, 0));
	AddBind(std::make_unique<Sampler>(gfx, 0, FILTER::FILTER_POINT_MIP_POINT));
	AddBind(std::make_unique<Blender>(gfx, false, std::nullopt));
	const glm::mat4 ConstantBuffer = glm::mat4(1.0);
	AddBind(std::make_unique<VertexConstantBuffer<glm::mat4>>(gfx, ConstantBuffer));

	//允许四种语义Position、Color、Normal、UV
	const std::vector<INPUT_ELEMENT_DESC> ied =
	{
		{"Position", sizeof(float), 12, 0},
		{"Color", sizeof(float), 12, 12},
		//{"TEXCOORD0", sizeof(float), 8, 24},
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied));
	AddBind(std::make_unique<Topology>(gfx, PRIMITIVE_TOPOLOGY::PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	initTransform = glm::mat4(1.0);
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void SolidSphere::SetPos(glm::vec3 pos)
{
	this->pos = pos;
}

glm::mat4 SolidSphere::GetTransformXM() const
{
	glm::mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, pos);
	transform = glm::scale(transform, glm::vec3(0.02, 0.02, 0.02));
	return transform;
}

glm::mat4 SolidSphere::GetInitTransform() const
{
	return initTransform;
}
