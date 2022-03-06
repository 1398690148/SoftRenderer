#include "Plane.h"
#include "Casli/Renderer/Drawable.h"
#include "Casli/Renderer/Texture.h"
#include "Casli/Renderer/VertexConstantBuffer.h"
#include "Casli/Renderer/Blender.h"
#include "Casli/Core/BlendState.h"
#include <glm/gtc/matrix_transform.hpp>

Plane::Plane(Graphics & gfx)
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
	textures = new Texture(gfx, "../src/Casli/Image/vase_plant.png", 1, 0);
	pVertexBuffer = new VertexBuffer(gfx, this->vertices);
	pIndexBuffer = new IndexBuffer(gfx, this->indices);
	std::vector<glm::mat4> matrix;
	matrix.push_back(glm::mat4(1.0));
	matrix.push_back(glm::mat4(1.0));
	matrix.push_back(glm::mat4(1.0));
	pConstantBuffer = new VertexConstantBuffer(gfx, 0, (unsigned char*)&matrix[0], sizeof(glm::mat4) * matrix.size());
	pBlender = new Blender(gfx, true);
}

void Plane::Bindable(Graphics & gfx, unsigned char *ConstantBuffer)
{
	std::vector<glm::mat4> v;
	glm::mat4 MVP = glm::mat4(1.0);
	glm::mat4 MT = glm::mat4(1.0);
	glm::mat4 M = glm::mat4(1.0);
	MVP = glm::scale(MVP, glm::vec3(2, 2, 1));
	MVP = gfx.GetProjection() * gfx.GetCamera() * MVP;
	v.push_back(MVP);
	v.push_back(M);
	v.push_back(MT);
	pConstantBuffer->SetMatrix(v);
	pVertexBuffer->Bind(gfx);
	pIndexBuffer->Bind(gfx);
	pConstantBuffer->Bind(gfx);
	GetContext(gfx)
	pBlender->Bind(gfx);
}

void Plane::Draw(Graphics &gfx)
{

	//for (int i = 0; i < textures.size(); i++)
	{
		textures->Bind(gfx);
	}
	gfx.Draw();
}
