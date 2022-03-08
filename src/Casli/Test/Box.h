#pragma once
#include <VertexBuffer.h>
#include <IndexBuffer.h>

class Graphics;

class RENDERER_API Box
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