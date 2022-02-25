#include "Plane.h"
#include "../Renderer/Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Renderer/VertexConstantBuffer.h"

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
	for (int i = 0; i < 30; i += 5)
	{
		vertex.Position = glm::vec3(v[i], v[i + 1], v[i + 2]);
		vertex.TexCoords = glm::vec2(v[i + 3], v[i + 4]);
		vertices.push_back(vertex);
	}
	for (int i = 0; i < 6; i++)
	{
		indices.push_back(i);
	}
	textures = new Texture(gfx, "../src/Casli/Image/wall_diffuse.jpg", 5, 0);
	pVertexBuffer = new VertexBuffer(gfx, this->vertices);
	pIndexBuffer = new IndexBuffer(gfx, this->indices);
	//matrix = glm::scale(matrix, glm::vec3(4, 4, 1));
	//matrix = glm::mat4(1.0);
	//pConstantBuffer = new VertexConstantBuffer(gfx, matrix);
}

void Plane::Draw(Graphics &gfx)
{
	//matrix = glm::mat4(1.0);
	//matrix = glm::translate(matrix, glm::vec3(0, -0.5, 0));
	//matrix = glm::rotate(matrix, glm::radians(-75.f), glm::vec3(1.f, 0.f, 0.f));
	//matrix = glm::scale(matrix, glm::vec3(2, 2, 1));
	//matrix = gfx.GetProjection() * gfx.GetCamera() * matrix;
	//pConstantBuffer->SetMatrix(matrix);
	pVertexBuffer->Bind(gfx);
	pIndexBuffer->Bind(gfx);
	pConstantBuffer->Bind(gfx);
	//for (int i = 0; i < textures.size(); i++)
	{
		textures->Bind(gfx);
	}
	gfx.Draw();
}

void Plane::SetMatrix(glm::mat4 mat)
{
	matrix = mat;
}
