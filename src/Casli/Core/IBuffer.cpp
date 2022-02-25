#include "IBuffer.h"
#include <memory>

IBuffer::IBuffer(unsigned int byteWidth, unsigned int structureByteStride) 
	: ByteWidth(byteWidth), StructureByteStride(structureByteStride)
{
}

IBuffer::IBuffer(unsigned int byteWidth, unsigned int structureByteStride, const void *buffer)
	: ByteWidth(byteWidth), StructureByteStride(structureByteStride)
{
	m_Buffer = new unsigned char[byteWidth];
	memcpy(m_Buffer, buffer, byteWidth);
}

IBuffer::~IBuffer()
{
	if (m_Buffer)
		delete m_Buffer;
}

unsigned char *IBuffer::GetBuffer(unsigned int offset)
{
	return m_Buffer + offset;
}

void IBuffer::GetUniqueBuffer(unsigned char **buffer)
{
	*buffer = m_Buffer;
	m_Buffer = nullptr;
}

inline unsigned int IBuffer::GetStructureByteStride() const
{
	return StructureByteStride;
}

inline unsigned int IBuffer::GetByteWidth() const
{
	return ByteWidth;
}
