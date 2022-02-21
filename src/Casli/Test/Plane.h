#pragma once
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/IndexBuffer.h"
class Graphics;
class Plane
{
public:
	Plane(Graphics &gfx);
	void Draw(Graphics &gfx);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture *> textures;
	VertexBuffer *pVertexBuffer{};
	IndexBuffer *pIndexBuffer{};
};