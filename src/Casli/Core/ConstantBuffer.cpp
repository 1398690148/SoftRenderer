#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(unsigned int byteStride, unsigned int structureByteStride, const void *buffer)
	: IBuffer(byteStride, structureByteStride, buffer)
{
}
