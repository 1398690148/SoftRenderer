#pragma once
#include <VertexBuffer.h>
#include <Drawable.h>

class Graphics;
class VertexConstantBuffer;
class Blender;
class IndexBuffer;

class RENDERER_API AlphaPlane : public Drawable
{
public:
	AlphaPlane(Graphics &gfx, std::string texturePath, int mipLevel, FILTER filter = FILTER_POINT_MIP_POINT);
	void Draw() override;
	void Bind(unsigned char *ConstantBuffer, size_t size) override;

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture *textures;
	VertexBuffer *pVertexBuffer{};
	IndexBuffer *pIndexBuffer{};
	VertexConstantBuffer *pConstantBuffer{};
	Sampler *sampler{};
	glm::mat4 matrix;
	Blender *pBlender{};
	SRBlendState *oldBlender;
	float oldBlendFactor;
	unsigned int oldSampleMask;
};