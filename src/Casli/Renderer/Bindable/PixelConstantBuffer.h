#pragma once
#include "Bindable.h"
#include <glm/mat4x4.hpp>

class RENDERER_API PixelConstantBuffer : public Bindable
{
public:
	PixelConstantBuffer(Graphics & gfx, unsigned int slot, unsigned char *buffer, int byteWidth);
	void Bind(Graphics &gfx);
	void SetMatrix(glm::mat4 matrix);
private:
	IBuffer *pConstantBuffer{};
	glm::mat4 matrix;
	std::vector<glm::vec3> cbuf;
	unsigned int StartOffset;
};