#pragma once
#include "Bindable.h"

class RENDERER_API VertexConstantBuffer : public Bindable
{
public:
	VertexConstantBuffer(Graphics &gfx, std::vector<glm::mat4> &matrix);
	VertexConstantBuffer(Graphics & gfx, unsigned int offset, unsigned char *buffer, int byteWidth);
	void Bind(Graphics &gfx);
	void SetConstantBuffer(unsigned char *ppConstantBuffer, unsigned int size);
private:
	SRIBuffer *pConstantBuffer{};
	std::vector<glm::mat4> matrix;
	unsigned char *cbuf{};
};