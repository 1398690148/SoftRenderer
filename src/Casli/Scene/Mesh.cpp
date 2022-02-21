#include "Mesh.h"
#include "Graphics.h"
#include "../Renderer/Texture.h"

Mesh::Mesh(Graphics &gfx, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	pVertexBuffer = new VertexBuffer(gfx, this->vertices);
	pIndexBuffer = new IndexBuffer(gfx, this->indices);
}

void Mesh::Draw(Graphics &gfx)
{
	pVertexBuffer->Bind(gfx);
	pIndexBuffer->Bind(gfx);
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i]->Bind(gfx);
	}
	gfx.Draw();
}
