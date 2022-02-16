#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(unsigned int byteStride, unsigned int structureByteStride, const void *buffer)
	: IBuffer(byteStride, structureByteStride, buffer)
{

}
