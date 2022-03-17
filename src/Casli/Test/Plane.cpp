#include <Plane.h>
#include <Texture.h>
#include <Sampler.h>
#include <Topology.h>
#include <InputLayout.h>
#include <SampleTextureVS.h>
#include <SampleTexturePS.h>
#include <VertexShader.h>
#include <PixelShader.h>
#include <TransformCbuf.h>
#include <glm/gtx/transform.hpp>

Plane::Plane(Graphics &gfx, std::string texturePath, int mipLevel, glm::vec3 translate, glm::vec4 rotate, FILTER filter) 
	: m_Translate(translate), m_Rotate(rotate)
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	Vertex vertex;
	float v[] = {
	-0.5f, -0.5f,  0.f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	std::vector<Vertex> vertices;
	for (int i = 0; i < 48; i += 8)
	{
		vertex.Position = glm::vec3(v[i], v[i + 1], v[i + 2]);
		vertex.Normal = glm::vec3(v[i + 3], v[i + 4], v[i + 5]);
		vertex.TexCoords = glm::vec2(v[i + 6], v[i + 7]);
		vertices.push_back(vertex);
	}
	std::vector<unsigned int> indices;
	for (int i = 5; i >= 0; i--)
	{
		indices.push_back(i);
	}
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	AddBind(std::make_unique<IndexBuffer>(gfx, indices));
	AddBind(std::make_unique<VertexShader>(gfx, new SampleTextureVS()));
	AddBind(std::make_unique<PixelShader>(gfx, new SampleTexturePS()));
	AddBind(std::make_unique<Texture>(gfx, texturePath.c_str(), mipLevel, 0));
	AddBind(std::make_unique<Sampler>(gfx, 0, filter));
	const glm::mat4 ConstantBuffer = glm::mat4(1.0);
	AddBind(std::make_unique<VertexConstantBuffer<glm::mat4>>(gfx, ConstantBuffer));

	//允许四种语义Position、Color、Normal、UV
	const std::vector<INPUT_ELEMENT_DESC> ied =
	{
		{"Position", sizeof(float), 12, 0},
		{"Normal", sizeof(float), 12, 12},
		{"TEXCOORD0", sizeof(float), 8, 24},
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied));
	AddBind(std::make_unique<Topology>(gfx, PRIMITIVE_TOPOLOGY::PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

glm::mat4 Plane::GetTransformXM() const
{
	return glm::translate(m_Translate) * glm::rotate(glm::radians(m_Rotate.x), glm::vec3(m_Rotate.y, m_Rotate.z, m_Rotate.w));
}
