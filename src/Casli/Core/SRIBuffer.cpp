#include "SRIBuffer.h"
#include <memory>

SRIBuffer::SRIBuffer(unsigned int byteWidth, unsigned int structureByteStride)
	: ByteWidth(byteWidth), StructureByteStride(structureByteStride)
{
}

SRIBuffer::SRIBuffer(unsigned int byteWidth, unsigned int structureByteStride, const void *buffer)
	: ByteWidth(byteWidth), StructureByteStride(structureByteStride)
{
	m_Buffer = new unsigned char[byteWidth];
	memcpy(m_Buffer, buffer, byteWidth);
}

SRIBuffer::~SRIBuffer()
{
	if (m_Buffer)
		delete m_Buffer;
}

unsigned char *SRIBuffer::GetBuffer(unsigned int offset)
{
	return m_Buffer + offset;
}

void SRIBuffer::GetUniqueBuffer(unsigned char **buffer)
{
	*buffer = m_Buffer;
	m_Buffer = nullptr;
}

inline unsigned int SRIBuffer::GetStructureByteStride() const
{
	return StructureByteStride;
}

inline unsigned int SRIBuffer::GetByteWidth() const
{
	return ByteWidth;
}
