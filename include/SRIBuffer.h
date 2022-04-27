#pragma once
#include "SRCoreAPI.h"

class CORE_API SRIBuffer
{
public:
	SRIBuffer(unsigned int byteWidth, unsigned int structureByteStride);
	SRIBuffer(unsigned int byteWidth, unsigned int structureByteStride, bool flag);
	SRIBuffer(unsigned int byteWidth, unsigned int structureByteStride, const void *buffer);
	virtual ~SRIBuffer();
	unsigned char *GetBuffer(unsigned int offset);
	void GetUniqueBuffer(unsigned char **buffer);
	unsigned int GetStructureByteStride() const;
	unsigned int GetByteWidth() const;
protected:
	unsigned char *m_Buffer{};
	unsigned int ByteWidth;
	unsigned int StructureByteStride;
};