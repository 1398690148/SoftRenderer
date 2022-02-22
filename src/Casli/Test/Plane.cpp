#include "Plane.h"
#include "../Renderer/Texture.h"

Plane::Plane(Graphics & gfx)
{
	Vertex vertex;
	float v[] = {
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
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
	textures = new Texture(gfx, "../src/Casli/Image/container.jpg", 0);
	pVertexBuffer = new VertexBuffer(gfx, this->vertices);
	pIndexBuffer = new IndexBuffer(gfx, this->indices);
}

void Plane::Draw(Graphics &gfx)
{
	pVertexBuffer->Bind(gfx);
	pIndexBuffer->Bind(gfx);
	//for (int i = 0; i < textures.size(); i++)
	{
		textures->Bind(gfx);
	}
	gfx.Draw();
}