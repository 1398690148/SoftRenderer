#pragma once
#include "CoreAPI.h"
#include <string>

enum BufferType
{
	BIND_VERTEX_BUFFER,
	BIND_INDEX_BUFFER,
	BIND_CONSTANT_BUFFER,
	BIND_RENDER_TARGET,
	BIND_DEPTH_STENCIL,
	BIND_SHADER_RESOURCE,
};

enum DataFormat
{
	FORMAT_INT,
	FORMAT_FLOAT,
};

typedef struct BUFFER_DESC
{
	BufferType BindFlags;
	unsigned int ByteWidth;
	unsigned int StructureByteStride;
} BUFFER_DESC;

typedef struct TEXTURE2D_DESC
{
	unsigned int Width;
	unsigned int Height;
	unsigned int MipLevels;
	unsigned int ArraySize;
	DataFormat Format;
	BufferType BindFlags;
} TEXTURE2D_DESC;

typedef struct VIEWPORT
{
	int Width;
	int Height;
	int TopLeftX;
	int TopLeftY;
	int MinDepth;
	int MaxDepth;
} VIEWPORT;

typedef struct INPUT_ELEMENT_DESC
{
	std::string SemanticName;
	unsigned int typeSize;
	unsigned int Size;
	unsigned int Offset;
} INPUT_ELEMENT_DESC;

typedef struct SUBRESOURCE_DATA
{
	const void *pSysMem;
	//从纹理的一行开始到下一行的距离
	unsigned int SysMemPitch;
} SUBRESOURCE_DATA;

enum PRIMITIVE_TOPOLOGY
{
	PRIMITIVE_TOPOLOGY_LINELIST,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST,
};

class CORE_API IBuffer
{
public:
	IBuffer(unsigned int byteWidth, unsigned int structureByteStride);
	IBuffer(unsigned int byteWidth, unsigned int structureByteStride, const void *buffer);
	virtual ~IBuffer();
	unsigned char *GetBuffer(unsigned int offset);
	unsigned int GetStructureByteStride() const;
	unsigned int GetByteWidth() const;
protected:
	unsigned char *m_Buffer;
	unsigned int ByteWidth;
	unsigned int StructureByteStride;
};