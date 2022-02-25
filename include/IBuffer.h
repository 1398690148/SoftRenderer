#pragma once
#include "CoreAPI.h"
#include "Core.h"

class CORE_API IBuffer
{
public:
	IBuffer(unsigned int byteWidth, unsigned int structureByteStride);
	IBuffer(unsigned int byteWidth, unsigned int structureByteStride, const void *buffer);
	virtual ~IBuffer();
	unsigned char *GetBuffer(unsigned int offset);
	void GetUniqueBuffer(unsigned char **buffer);
	unsigned int GetStructureByteStride() const;
	unsigned int GetByteWidth() const;
protected:
	unsigned char *m_Buffer{};
	unsigned int ByteWidth;
	unsigned int StructureByteStride;
};