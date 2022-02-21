#pragma once
#include "Bindable.h"
#include <glm/mat4x4.hpp>

class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics &gfx, glm::mat4 &matrix);
	void Bind(Graphics &gfx);
private:
	IBuffer *pConstantBuffer{};
	glm::mat4 matrix;
};