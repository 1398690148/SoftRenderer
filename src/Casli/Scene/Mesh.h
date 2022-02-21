#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/IndexBuffer.h"

class Texture;
class Graphics;

class Mesh
{
public:
	Mesh(Graphics &gfx, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture *> textures);
	void Draw(Graphics &gfx);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture *> textures;
	VertexBuffer *pVertexBuffer{};
	IndexBuffer *pIndexBuffer{};
};