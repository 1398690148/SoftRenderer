#pragma once
#include "Bindable.h"
#include <glm/mat4x4.hpp>

class RENDERER_API PixelConstantBuffer : public Bindable
{
public:
	PixelConstantBuffer(Graphics &gfx, unsigned char *buffer, unsigned int slot, unsigned int ByteWidth);
	void ResetBuffer(unsigned char *buffer);
	void Bind(Graphics &gfx);
private:
	IBuffer *pConstantBuffer{};
	unsigned int offset = 0;
};