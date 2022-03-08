#pragma once
#include "Bindable.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class RENDERER_API VertexBuffer : public Bindable
{
public:
	VertexBuffer(Graphics &gfx, std::vector<Vertex> &vertex);
	void Bind(Graphics &gfx);
private:
	IBuffer *pVertexBuffer{};
};