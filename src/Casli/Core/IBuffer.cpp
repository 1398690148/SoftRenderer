#include "IBuffer.h"
#include <memory>

IBuffer::IBuffer(unsigned int byteWidth, unsigned int structureByteStride) : ByteWidth(byteWidth), StructureByteStride(structureByteStride)
{
}

IBuffer::IBuffer(unsigned int byteWidth, unsigned int structureByteStride, const void *buffer)
	: ByteWidth(byteWidth), StructureByteStride(structureByteStride)
{
	m_Buffer = new unsigned char[byteWidth];
	if (buffer != nullptr)
		memcpy(m_Buffer, buffer, byteWidth);
	else
		memset(m_Buffer, -1, byteWidth);
}

IBuffer::~IBuffer()
{
	if (m_Buffer)
		delete m_Buffer;
}

unsigned char * IBuffer::GetBuffer(unsigned int offset)
{
	return m_Buffer + offset;
}

inline unsigned int IBuffer::GetStructureByteStride() const
{
	return StructureByteStride;
}

inline unsigned int IBuffer::GetByteWidth() const
{
	return ByteWidth;
}
