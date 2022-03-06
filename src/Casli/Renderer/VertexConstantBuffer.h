#pragma once
#include "Bindable.h"

class VertexConstantBuffer : public Bindable
{
public:
	VertexConstantBuffer(Graphics &gfx, std::vector<glm::mat4> &matrix);
	VertexConstantBuffer(Graphics & gfx, unsigned int offset, unsigned char *buffer, int byteWidth);
	void Bind(Graphics &gfx);
	void SetMatrix(std::vector<glm::mat4> matrix);
private:
	IBuffer *pConstantBuffer{};
	std::vector<glm::mat4> matrix;
	unsigned char *cbuf{};
};