#pragma once
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Drawable.h>

class Graphics;
class VertexConstantBuffer;
class Blender;

class RENDERER_API Plane : public Drawable
{
public:
	Plane(Graphics &gfx, std::string texturePath, int mipLevel, FILTER filter = FILTER_POINT_MIP_POINT);
	void Draw(Graphics &gfx) override;
	void Bind(Graphics &gfx, unsigned char *ConstantBuffer,size_t size) override;

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture *textures;
	VertexBuffer *pVertexBuffer{};
	IndexBuffer *pIndexBuffer{};
	VertexConstantBuffer *pConstantBuffer{};
	Sampler *sampler{};
	glm::mat4 matrix;
	//Blender *pBlender{};
	//BlendState *oldBlender;
	//float oldBlendFactor;
	//unsigned int oldSampleMask;
};