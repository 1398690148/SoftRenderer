#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"

class CORE_API ConstantBuffer : public IBuffer
{
public:
	ConstantBuffer(unsigned int byteStride, unsigned int structureByteStride, const void *buffer);
};