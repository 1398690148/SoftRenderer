#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics &gfx, std::vector<unsigned int> &indices);
	void Bind(Graphics &gfx);
private:
	SRIBuffer *pIndexBuffer{};
};