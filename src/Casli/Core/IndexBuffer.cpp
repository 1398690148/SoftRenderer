#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(unsigned int byteStride, unsigned int structureByteStride, const void * buffer)
	: IBuffer(byteStride, structureByteStride, buffer)
{
}
