#pragma once
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/IndexBuffer.h"
class Graphics;
class VertexConstantBuffer;
class Plane
{
public:
	Plane(Graphics &gfx);
	void Draw(Graphics &gfx);
	void SetMatrix(glm::mat4 mat);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture * textures;
	VertexBuffer *pVertexBuffer{};
	IndexBuffer *pIndexBuffer{};
	VertexConstantBuffer *pConstantBuffer{};
	glm::mat4 matrix;
};