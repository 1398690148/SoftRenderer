#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"

class CORE_API IndexBuffer : public IBuffer
{
public:
	IndexBuffer(unsigned int byteStride, unsigned int structureByteStride, const void *buffer);
private:
	unsigned int stride;
	unsigned int offset;
};