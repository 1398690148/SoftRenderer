#include "AlphaPlane.h"
#include <Texture.h>
#include <VertexConstantBuffer.h>
#include <Blender.h>
#include <IndexBuffer.h>

AlphaPlane::AlphaPlane(Graphics & gfx, std::string texturePath, int mipLevel)
{
	Vertex vertex;
	float v[] = {
	-0.5f, -0.5f,  0.f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	for (int i = 0; i < 48; i += 8)
	{
		vertex.Position = glm::vec3(v[i], v[i + 1], v[i + 2]);
		vertex.Normal = glm::vec3(v[i + 3], v[i + 4], v[i + 5]);
		vertex.TexCoords = glm::vec2(v[i + 6], v[i + 7]);
		vertices.push_back(vertex);
	}
	for (int i = 0; i < 6; i++)
	{
		indices.push_back(i);
	}
	textures = new Texture(gfx, texturePath.c_str(), mipLevel, 0);
	pVertexBuffer = new VertexBuffer(gfx, this->vertices);
	pIndexBuffer = new IndexBuffer(gfx, this->indices);
	std::vector<glm::mat4> matrix;
	matrix.push_back(glm::mat4(1.0));
	matrix.push_back(glm::mat4(1.0));
	matrix.push_back(glm::mat4(1.0));
	pConstantBuffer = new VertexConstantBuffer(gfx, 0, (unsigned char*)&matrix[0], sizeof(glm::mat4) * matrix.size());
	pBlender = new Blender(gfx, true, {});
}

void AlphaPlane::Bind(Graphics &gfx, unsigned char *ConstantBuffer, size_t size)
{
	pVertexBuffer->Bind(gfx);
	pIndexBuffer->Bind(gfx);
	pConstantBuffer->SetConstantBuffer(ConstantBuffer, size);
	pConstantBuffer->Bind(gfx);
	GetContext(gfx)->OMGetBlendState(&oldBlender, &oldBlendFactor, &oldSampleMask);
	pBlender->Bind(gfx);
}

void AlphaPlane::Draw(Graphics &gfx)
{
	//for (int i = 0; i < textures.size(); i++)
	{
		textures->Bind(gfx);
	}
	gfx.Draw();
	GetContext(gfx)->OMSetBlendState(&oldBlender, &oldBlendFactor, oldSampleMask);
}
