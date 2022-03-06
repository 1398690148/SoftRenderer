#pragma once
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/Drawable.h"

class Graphics;
class VertexConstantBuffer;
class Blender;

class Plane : public Drawable
{
public:
	Plane(Graphics &gfx);
	void Bindable(Graphics &gfx, unsigned char *ConstantBuffer);
	void Draw(Graphics &gfx);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture * textures;
	VertexBuffer *pVertexBuffer{};
	IndexBuffer *pIndexBuffer{};
	VertexConstantBuffer *pConstantBuffer{};
	glm::mat4 matrix;
	Blender *pBlender{};
	BlendState *oldBlender;
	float *oldBlendFactor;
	float oldBlendMask;
};