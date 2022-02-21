#pragma once
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/IndexBuffer.h"
class Graphics;
class Box
{
public:
	Box(Graphics &gfx);
	void Draw(Graphics &gfx);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture *> textures;
	VertexBuffer *pVertexBuffer{};
	IndexBuffer *pIndexBuffer{};
};