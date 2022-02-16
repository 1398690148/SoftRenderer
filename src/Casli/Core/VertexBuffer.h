#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"

class CORE_API VertexBuffer : public IBuffer
{
public:
	VertexBuffer(unsigned int byteStride, unsigned int structureByteStride, const void *buffer);
private:
	unsigned int stride;
	unsigned int offset;
};